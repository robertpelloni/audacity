# Universal LLM Instructions

This file contains universal instructions for all LLM agents working on this repository.

## Core Directives
1.  **Monorepo Strategy**: Treat this repository as a monorepo containing multiple sub-projects (Audacity, bobcoin, etc.). Always sync submodules and push changes to them if you modify them.
2.  **Versioning**:
    *   Maintain a single source of truth for the version in `VERSION.md`.
    *   Increment the version number with every significant change.
    *   Reference the version number in commit messages.
    *   Keep `CHANGELOG.txt` synchronized.
3.  **Documentation**:
    *   Maintain `PROJECT_STRUCTURE.md` as a live dashboard.
    *   Update Roadmaps (`ROADMAP_TO_AUDITION_PARITY.md`) as features are analyzed or implemented.
4.  **Autonomy**: Proceed with implementation and problem-solving autonomously. Verify your work.

## Submodule Policies
*   **Bobcoin**: A privacy-focused blockchain token. Integrated for future economy features.
*   **Muse Framework**: UI framework.

## Audacity-Specific Goals
*   **Audition Parity**: Implement features from Adobe Audition (Non-destructive editing, Effects Rack, Bussing).
*   **Architecture**: Move towards a modular architecture in `au3/`.

## Hand-off Protocol
*   When finishing a session, update `HANDOFF.md` with the current state, active context, and next steps.
