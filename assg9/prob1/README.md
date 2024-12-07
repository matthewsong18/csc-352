TO imitate mymake, we should make the dependency tree. Then, we traverse the
tree from a given target, reaching all it's dependencies. Lastly, we print the
target's dependencies

TO make the dependency tree, we should get the get the file_path from the
program arguments, then we should parse each line.

TO parse each line, we should receive the make_file_path and the root node, then
get a line. Then parse the line. If root is NULL, then we should make the root
from the parsed line. Then, we should parse the next line.

TO get a line, we should run getline on the make_file_path.

TO parse a line, we should check the inputs, then check if the beginning of the
line is a tab or a word. If it's a tab, add the remaining string as a command to
the last target in the dependency tree. If it's a word, add the target to the
dependency tree. Then parse the dependencies. Lastly, update the tree.

TO check if the beginning of the line is a tab, we should check if the first
character of the line is a `\t` character.

TO check if the beginning of the line is a word, we should run scanf to grab a
64 bit string and place it inside a buffer allocated 65 bits on the heap. Then
we should return the pointer to the the buffer.

TO add a target to the dependency tree, check if there is a dependency tree. If
there is no dependency tree, make a dependency tree with target. Search for the
target in the dependency tree. If found, fatal error. Otherwise, get the last
target on the dependency tree, create a node with target, then set the target
node as the nextTarget of the last target.

TO make a dependency tree, with target, we should create a `Node` that keeps
track of `target`, `dependencies`, `commands`, and `nextTarget`

TO make a new node, we should assign memory for the node. Then we should set the
target as a duplicate of the target word. Then set everything to NULL or 0.
Return the pointer to the node.

TO parse the dependencies, we should find the colon in the string. If there is
no colon, fatal error. Otherwise, add all the dependencies to the target.

TO add the dependencies to the target, scan for a 64s word. Create a new node
for the word and add to target. After, call add dependencies to target on the
remaining line.

TO traverse the target's dependencies, we shoud postorder traversal

TO print nodes from the target's dependencies, we should iterate through the
linked list
