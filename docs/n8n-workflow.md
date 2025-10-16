# Automating Blockforge builds with n8n

This guide demonstrates how to register a reusable n8n workflow from Node.js that triggers
external build infrastructure for Blockforge.

The workflow created by the helper script contains three nodes:

1. **Manual Trigger** – lets you fire the automation directly from the n8n UI.
2. **Prepare Build Context** – a Function node that emits the command line to execute.
3. **Trigger Build Webhook** – posts the command payload to a webhook (for example, a self-hosted
   build runner that translates the payload into PowerShell commands like `tools/build.ps1 -Config Debug`).

The workflow stays inactive so it can be wired into more complex scenarios such as scheduled or
Git-triggered automation.

## Prerequisites

- A running n8n instance (self-hosted or cloud) with an API key enabled.
- Node.js 18 or later to run the helper script.

## Registering the workflow

```bash
# Export the API key n8n generated for you
export N8N_API_KEY=your-n8n-api-key

# Optionally change the host if n8n is not on localhost:5678
export N8N_HOST=https://automation.example.com

# Provide the webhook endpoint that will accept build requests
node tools/n8n/register-blockforge-workflow.mjs --webhook=https://ci.example.com/build
```

The script performs an upsert:

- If a workflow named **Blockforge Build Dispatcher** does not exist, it is created.
- If it already exists, the definition is updated in-place so the automation stays in sync with the
  repository version.

You can customize the workflow name with `--name="Custom Workflow"` and override the host or API key via
command line arguments instead of environment variables.

## Using the workflow output

The final HTTP Request node posts a JSON payload to the webhook you provide. It uses the structure below:

```json
{
  "command": "tools/build.ps1 -Config Debug",
  "project": "Blockforge"
}
```

Adapt your receiver to translate this payload into the appropriate build, test, or deployment actions.
Because the workflow definition is stored as code, you can version it along with the rest of the
Blockforge repository and reuse it across n8n environments.
