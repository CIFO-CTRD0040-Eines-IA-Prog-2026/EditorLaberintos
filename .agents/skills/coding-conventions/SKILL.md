---
name: coding-conventions
description: Coding convetions for the project, should be applied to all generated code
---
## Naming convention
- Always use camel case (starting with upper-case) except for macros. In that case all caps snake-case.
- Names must be descriptive of the functionality
### Variables
- All variables prefixed acoording to their type:
  - Integer: i
  - Unsigned integer: u
  - Float: f
  - String or pointer to characters: s
  - Pointer: p
  - Enum: e
  - Lambdas: fn
  - In any other case: o
- Parameter names: Same as variables but appending a single underscore as a perfix
- Class / struct members: Same as variables but appending prefix "m_"

## Code separation
Each class/struct on a dedeicated h file. If they contain methods, the implementation in a matching CPP file

## Const correctness
- Tag variables as const when they won't be modified except for by value parameters
- Tag as const all methods that do not modify object data

## Dependency injection
Do not create global objects. Pass required objects as parameters.