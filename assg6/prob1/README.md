# Assignment 6 Anagrams2

## Goal

Split words into groups of words that are anagrams of each other and print a line per group in the same order they appeared

## Input

Strings are on multiple lines with whitespace between them

## Base cases

- Make sure buffer protects for 64 length strings
- Must be an alphabetic characters

## Error handling

- Do not crash
- Print error message on invalid inputs and skip to next string
- Error status should be 1 if an error was seen

## Requirements

- No giant arrays
- All unique anagrams must be in a linked list
- Strings matching an anagram must be in a linked list

## Approach

- Grab one `string` from the input stream
- Check if the `string` matches an anagram in the `anagram` linked list
- If it matches, create a new node for the `string` at the tail of the `string_match` linked list
- If there is no anagram matching the `string`, add a new anagram node at the tail of the `anagram` linked list 

