# C Coding Style Skill

## Description

Enforces consistent C coding style: camelCase naming and Egyptian brace style.

## When to Use

- When importing or editing C files
- When creating new C source files
- When reviewing code for style compliance
- Before committing changes to ensure consistency

## Rules

### 1. File Headers
- All files must start with a comment containing the filename and "Example code for TDT4258 - 2026 edition"

### 2. Naming Conventions
- Use **camelCase** for all identifiers (functions, variables, types)
- Do NOT use snake_case
- Short names without any uppercase letters are allowed
- Examples: `calculateSum`, `maxValue`, `userCount`, `add`, `subtract`

### 3. Brace Style (Egyptian)
- Opening brace `{` on the same line as declaration/control statement
- Closing brace `}` on its own line at the same indentation level
- No extra blank lines between braces

## Examples

### GOOD - Egyptian style
```c
// GOOD - Egyptian style
int calculateSum(int a, int b) {
    return a + b;
}

if (x > 0) {
    printf("positive\n");
}
```
### BAD - Other styles
```c
// BAD - Other styles
int calculate_sum(int a, int b)
{
    return a + b;
}

if (x > 0)
{
    printf("positive\n");
}
```