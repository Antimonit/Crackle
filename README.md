# Crackle

Crackle is a statically typed interpreter with similar syntax
to Java and is written in C++ using Flex and Bison tools.

The language is named Crackle after the fifth derivative of position.

### Types

Crackle follows similar grammar rules as the Java language.
Crackle is, similarly to Java, statically typed and so every variable
assignment has to assign appropriate value, functions have to
be called with declared parameter types, etc.

#### Primitive types
For simplicity reasons, Crackle supports only following primitive types:
```integer```, ```double```, ```string```, ```boolean``` and ```void```.
```
int size = 42;
double pi = 3.14159265;
string name = "David";
boolean visible = true;
void nothing; // cannot assign a value
```
If definition is not provided together with declaration, primitive types
are by default initialized to their usual empty values
(0 for integers, 0.0 for doubles, "" for strings, false for booleans)

#### Objects
Programmer may define object types composed of primitive types and
object types (including recursively defined objects) with ```object```
keyword.

```
object Node {
    int depth = 1;
    string value;
    Node next;
}
```

Objects can be initialized using ```new``` keyword which allocate
the object on the heap.
By default, if no definition is provided, objects are initialized
to ```null``` denoting a missing value.

### Language features

#### Operators

Crackle supports:
* basic **arithmetic operators** (+ - * / %) for integers and doubles _(modulo only for integers)_.
* **comparison operators** (< <= > >= == !=) for integer and doubles
* **boolean operators** (&& || !) for booleans
* **unary operators** (- -- ++) for integers and doubles

#### Control flow statements

Crackle supports:
* **if statement**
* **while statement**
* **for statement**

All flow statements must be defined with curly braces, not doing so
produces syntax error.

*Example code:*
```
while (node != null) {
    if (node.value > 10) {
        println "Node is too big"
    } else {
        int i;
        for (i = 0; i < node.value; i++) {
            println i
        }
    }
    node = node.parent;
}
```

#### Parametrized functions
Programmer can define functions with zero or more parameters.
When a function is called, its parameter count and parameter types must
match, otherwise a warning is produced.

### Scope
Declaring two variables (or functions) with a same name in the same scope produces a
warning and the later declaration is not executed.

Programmer still can declare variables (or functions) with the same
name in different scopes. In such case, whenever a variable (or a
function) is referenced, the variable (or function) defined as
inner-most to the current scope is retrieved and used.

#### Local variables and functions
As in Java, programmer can declare local variables inside of functions.
On the contrary to Java, it is also possible to
**define local functions within other functions**. The same way as
local variables cease to exist when the surrounding function returns,
the inner function declaration ceases to exist when the outer function
returns.

### Runtime

#### Garbage Collection

Objects allocated using ```new``` keyword are stored in the heap.
The interpret contains simple Mark-and-Sweep GC that is run whenever
the maximum amount of objects has been allocated.

#### Target code

Crackle does not generate any intermediate code and all statements
are stored in a form of Abstract Syntax Tree which is also used to
evaluate expression and functions.

### How to run
By default, the program gets its input from the standard input and
prints output to the standard output. This can be changed with command
line arguments:
* **-i <file_name>** read input from the <file_name>
* **-o <file_name>** writes output to the <file_name>
* **-d <file_name>** writes debug information to the <file_name>
* **-f** writes debug to the standard output
* **--heap <heap_size>** sets the heap size to <heap_size>
* **--heap <heap_size>** sets the heap size to <heap_size>

### How to compile
Crackle uses Flex scanner and Bison parser tools to facilitate the
lexical and syntax analysis.
Crackle is compiled using CMake. CMake handles Flex and Bison
dependencies by itself and one does not have to provide the binaries.
CMake file is a part of source code.

### To be implemented

* Comments
* Function reference as a type -> higher order functions
* Arrays
* Crash nicely when OOM
* Fix leaks
* Unit tests
* Rename object to record (https://en.wikipedia.org/wiki/Record_(computer_science))