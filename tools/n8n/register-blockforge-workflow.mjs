#!/usr/bin/env node

const args = process.argv.slice(2);
const options = {};
for (const arg of args) {
  const [key, value] = arg.split('=');
  if (key.startsWith('--')) {
    options[key.slice(2)] = value ?? true;
  }
}

const host = options.host ?? process.env.N8N_HOST ?? 'http://localhost:5678';
const apiKey = options['api-key'] ?? process.env.N8N_API_KEY;
if (!apiKey) {
  console.error('Missing n8n API key. Provide via --api-key=<key> or N8N_API_KEY env variable.');
  process.exit(1);
}

const workflowName = options.name ?? 'Blockforge Build Dispatcher';

const workflowPayload = {
  name: workflowName,
  active: false,
  nodes: [
    {
      parameters: {},
      id: '1',
      name: 'Manual Trigger',
      type: 'n8n-nodes-base.manualTrigger',
      typeVersion: 1,
      position: [260, 300],
    },
    {
      parameters: {
        functionCode: `return [{\n  buildCommand: 'tools/build.ps1 -Config Debug',\n  notes: 'Invoke from a Windows shell configured for vcpkg toolchain.',\n}];`,
      },
      id: '2',
      name: 'Prepare Build Context',
      type: 'n8n-nodes-base.function',
      typeVersion: 1,
      position: [540, 300],
    },
    {
      parameters: {
        url: options.webhook ?? 'http://localhost:3000/build',
        method: 'POST',
        jsonParameters: true,
        options: {},
        bodyParametersJson: `{"command":"tools/build.ps1 -Config Debug","project":"Blockforge"}`,
      },
      id: '3',
      name: 'Trigger Build Webhook',
      type: 'n8n-nodes-base.httpRequest',
      typeVersion: 2,
      position: [820, 300],
    },
  ],
  connections: {
    'Manual Trigger': {
      main: [
        [
          {
            node: 'Prepare Build Context',
            type: 'main',
            index: 0,
          },
        ],
      ],
    },
    'Prepare Build Context': {
      main: [
        [
          {
            node: 'Trigger Build Webhook',
            type: 'main',
            index: 0,
          },
        ],
      ],
    },
  },
};

const headers = {
  'Content-Type': 'application/json; charset=utf-8',
  'X-N8N-API-KEY': apiKey,
};

const fetchJson = async (url, init = {}) => {
  const response = await fetch(url, init);
  if (!response.ok) {
    const detail = await response.text();
    throw new Error(`Request failed (${response.status} ${response.statusText}): ${detail}`);
  }
  return response.json();
};

const baseUrl = host.replace(/\/$/, '') + '/api/v1';

let existingWorkflow;
try {
  const { data: workflows } = await fetchJson(`${baseUrl}/workflows`, {
    headers,
  });
  existingWorkflow = workflows.find((wf) => wf.name === workflowName);
} catch (error) {
  console.error('Unable to query existing workflows from n8n.');
  console.error(error.message);
  process.exit(1);
}

try {
  if (existingWorkflow) {
    const updated = await fetchJson(`${baseUrl}/workflows/${existingWorkflow.id}`, {
      method: 'PATCH',
      headers,
      body: JSON.stringify({ ...workflowPayload, id: existingWorkflow.id }),
    });
    console.log(`Updated existing workflow "${updated.name}" (id=${updated.id}).`);
  } else {
    const created = await fetchJson(`${baseUrl}/workflows`, {
      method: 'POST',
      headers,
      body: JSON.stringify(workflowPayload),
    });
    console.log(`Created workflow "${created.name}" (id=${created.id}).`);
  }
} catch (error) {
  console.error('Failed to upsert workflow.');
  console.error(error.message);
  process.exit(1);
}
