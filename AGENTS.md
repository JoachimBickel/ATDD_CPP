# AGENTS.md

> This file is the single source of project knowledge and agent guidance — keep
> all of it in here and in the code/config, and keep it up to date. Do **not**
> rely on per-machine agent "memory" features; that state does not travel with
> the repository.

## Project

A small cross-platform **3D viewer application** written in **C++**.

- **GUI framework:** Qt
- **Build system:** CMake
- **Test framework:** Boost.Test
- **Targets:** Cross-platform — at minimum **Windows** and **Linux**, for **both development and execution**.

Keep all code, build configuration, and tooling portable across these platforms. Avoid platform-specific APIs unless they are abstracted behind a portable interface, and prefer CMake/Qt facilities that work identically on Windows and Linux.

## Architecture

**Hexagonal (ports & adapters).** A pure, Qt-free **core** sits at the centre; the UI is an **adapter** plugged into ports the core defines. Dependencies point **inward** (`adapters → core`); the core depends on nothing external.

```
core/                      ← pure C++, no Qt; fully covered by Boost.Test
  domain/      geometry (Vec3, Mesh, Triangle, BoundingBox), analysis (metrics)
  app/         ViewerService (inbound ports), Camera, Picker, presentation logic
  ports/       inbound  — command interfaces the UI calls
               outbound — View + ModelSource interfaces the adapters implement
adapters/qt/   MainWindow, Viewport (QOpenGLWidget) implementing the View port;
               file reader implementing ModelSource. Built pragmatically, NOT via TDD.
tests/         Boost.Test; drives inbound ports against fake outbound ports
```

**Two ports for the UI:**

- **Inbound (commands)** — the Qt adapter calls these: `openModel(path)`, `orbit(dx,dy)`, `zoom(d)`, `pick(x,y)`, `selectMetric(...)`.
- **Outbound (view)** — the core pushes view-state out through an interface Qt implements: `renderMesh(RenderData)`, `showMetrics(...)`, `showError(...)`. Tests substitute a fake view and assert on it.

The Qt view is **humble/passive**: translate input events into inbound commands, render whatever view-state the core supplies, and hold almost no logic of its own.

**Rules that keep the core testable and portable:**

- **Port interfaces use only core/domain types** (our `Vec3`, plain `RenderData`/`CameraState` structs, `std::string`) — **never Qt types** (`QVector3D`, `QMouseEvent`, …). The adapter translates Qt ↔ core types at the boundary.
- **Decisions/transformations live in the core; tech mechanics live in the adapter.** Don't route trivial pass-throughs through ceremony, but camera math, picking, parsing, and presentation logic belong in the core (and are prime TDD targets). Only GL draw calls and raw event capture stay in Qt.
- **Parsing is core logic** that works on in-memory text/streams (testable without touching the filesystem); reading bytes from disk is an adapter concern (`ModelSource`).

## Development methodology: TDD

Use **Test-Driven Development throughout**, following strict **red-green-refactor**:

1. **Red** — Write the *minimal* failing test that expresses the next small piece of desired behavior.
2. **Green** — Write the *minimal* implementation needed to make that test pass. No more than required.
3. **Refactor** — Improve the design without changing behavior, keeping tests green.

**TDD scope:** the core. The UI adapter is deliberately built pragmatically, without TDD — UI mechanics are a poor fit for it, and it holds no logic worth driving by tests; anything algorithmic belongs in the core.

### Test design

Prefer **integrated, high-level tests** that exercise behavior through public interfaces over fine-grained unit tests. High-level tests stay stable as the implementation changes, so they survive refactoring instead of breaking on it. Write a unit test only where it gives a clear benefit and won't turn fragile over time — e.g. isolating tricky algorithmic or edge-case logic that is hard to drive from the outside.

**Test organization:** one Boost.Test suite per file, named after the behavior area it covers (e.g. `loading_models` in `open_model_test.cpp`, `camera_interaction` in `camera_interaction_test.cpp`). Shared fakes and fixtures live in `tests/support/`. New tests go in the file whose behavior area they belong to — or a new file when they start a new area.

### Working agreements for the agent

- **Do not run tests or the application yourself.** When a test or build is expected to be red or green, **ask the user** to run it and report whether it is red or green (or what the failure is).
- **During the refactor step**, propose your own refactoring ideas **and explicitly ask the user** for additional refactorings they would like to see before proceeding.
- **Commit after every step** — after each **red**, each **green**, and each **refactor** step. Use clear commit messages indicating which step it is (e.g. `red: ...`, `green: ...`, `refactor: ...`).
- **Explain before changing tooling/config/environment.** Before applying changes to git config, build tooling, or the environment, explain what each change does (including what specific settings mean) and why, then get the go-ahead. Prefer non-destructive, verifiable steps — clarity matters more than speed.

### Step checklist

- [ ] Write minimal failing test (**red**) → ask user to confirm it fails → commit
- [ ] Write minimal implementation (**green**) → ask user to confirm it passes → commit
- [ ] Refactor: suggest improvements, ask user for more → apply → ask user to confirm still green → commit
