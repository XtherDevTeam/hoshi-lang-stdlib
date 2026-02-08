# tsuki-server

tsuki-server is a high-performance, lightweight remote package registry for the **hoshi-lang** ecosystem. It provides the backend infrastructure for the `tsuki` package manager, enabling seamless package distribution, version management, and artifact hosting.

## Features

- **Package Management**: Register, update, and search for packages.
- **Version Control**: Manage multiple versions and revision notes for each package.
- **Artifact Hosting**: Upload and download platform-specific binary artifacts (e.g., `.zip` files).
- **Security**: JWT-based authentication and flexible user permission system (Administrator/Member).
- **Admin API**: Remote configuration management and user administration.
- **RESTful API**: Intuitive endpoints for both the `tsuki` CLI and potential web frontends.
- **Database Backend**: Reliable storage using SQLite.

## Prerequisites

- [hoshi-lang compiler](https://github.com/XtherDevTeam/hoshi-lang) (via GitHub Releases)
- [tsuki package manager](https://github.com/XtherDevTeam/hoshi-lang-stdlib/tree/main/src/tsuki)

## Build & Run

tsuki-server is managed and built using the `tsuki` package manager itself.

### 1. Build the Server

Navigate to the `tsuki-server` directory and run:

```bash
tsuki build
```

The compiled binary will be located at `bin/tsuki-server`.

### 2. Configure and Run

Before starting the server, ensure you have a `blob` directory for database and artifact storage:

```bash
mkdir -p blob/artifacts
```

Run the server:

```bash
./bin/tsuki-server
```

On first run, the server will:
1. Initialize the SQLite database (`blob/database.db`) using the provided schema.
2. Prompt you to create an administrator user (`jerry_chou`) and set a password.

## Project Structure

- `main.hoshi`: The main entry point and API route definitions.
- `share/`: Core business logic and database abstractions.
    - `dataProvider.hoshi`: Database interaction layer.
    - `scheme.sql`: SQLite database schema.
    - `types.hoshi`: Common data structures and enums.
- `template/`: Default assets (e.g., default user avatar).
- `blob/`: (Runtime) Directory for database and uploaded artifacts.

## API Overview

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/` | GET | Server status and version info. |
| `/v1/user/login` | POST | Authenticate and get session cookie. |
| `/v1/package/list` | GET | List all registered packages. |
| `/v1/package/info` | POST | Get detailed information for a package. |
| `/v1/artifact/download/:id` | GET | Download a specific artifact. |
| `/v1/admin/*` | - | Administrative endpoints (Require Administrator privileges). |

## License

This project is licensed under the **MIT License**.
