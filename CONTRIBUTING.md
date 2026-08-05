# Contributing to Galara

Galara welcomes transparent, reviewable contributions.

## Rules

- Open an issue before making major consensus changes.
- Keep consensus changes separate from branding or documentation changes.
- Document the upstream behavior being changed.
- Add tests for subsidy, difficulty, validation, and serialization changes.
- Never commit passwords, private keys, seed phrases, signing keys, or server credentials.
- Do not add founder controls, hidden issuance, transaction blacklists, or balance-seizure functions.
- Require review before merging security-sensitive code.
- Use clear commit messages.

## Consensus changes

Every consensus pull request must explain:

1. the exact rule being changed;
2. activation conditions;
3. compatibility impact;
4. test coverage;
5. supply or security consequences;
6. rollback or recovery considerations.
