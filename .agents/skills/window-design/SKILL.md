---
name: window-design
description: Visual style guide for the application window
---

# Style guide

This is the style guide for the application UI. Do not generate additional colors

## Pallete

| Token              | Hex       | Usage                                     |
|--------------------|-----------|-------------------------------------------|
| `bg`               | `#343a40` | Window barckground color                  |
| `ink`              | `#ffd43b` | Color for text directly over the window   |
| `button_bg`        | `#868e96` | Background color for buttons              |
| `button_ink`       | `#ffd43b` | Color for text over buttons               |
| `panel_head_bg`    | `#868e96` | Backgorund color for panel header         |
| `panel_head_ink`   | `#ffd43b` | Color for text on the panel header        |
| `panel_bg`         | `#ced4da` | Background color for the panel            |
| `field_bg`         | `#343a40` | Background color for fields               |
| `field_ink`        | `#e03131` | Text color for fields text                |

## Font and hierarchy
- Use default ImGUI font
- Fields in the same hierarchy level must be aligned to an axis so:
    - Labels are left-aligned
    - Fields are right-aligned
- Buttons and panels must auto-resize to fill their conatiners horizontal area