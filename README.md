# dre
Dreadful Regex Engine

# Cases Handled

- **CARET symbol (^):** matching beggining of string
- **DOLLAR symbol ($):** matching end of string
- **DOT symbol (.):** matching any character
- **STAR symbol (*):** matching the preceding character 0 or more times
- **PLUS symbol (+):** matching the preceding character 1 or more times
- **QUESTION symbol (?):** matching the preceding character 0 or 1 time
- **DIGIT (\d):** matching digits 0-9 
- **ALPHA (\w):** matching alphabets a-zA-Z
- **CHAR_CLASS ([...]):** matching any characters in the set

# Usage
The engine provides a match function used as:
```
void match(char *pattern, char *text)
```
It furthur compiles the regex pattern into a form usable in the next stages. 
