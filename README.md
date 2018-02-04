# Crackle

Crackle is a statically typed interpreter with similar syntax
to Java and is written in C++ using Flex and Bison tools.

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

* Basic **arithmetic operators** (+ - * / %) for integers and doubles _(modulo only for integers)_
with automatic expansion to double type.

    ```
    int addition = 5 + 3;	// 8
    int subtraction = 5 - 3;	// 2
    int multiplication = 5 * 3;	// 15
    int division = 5 / 3;	// 1
    int modulo = 5 % 3;	// 2

    double addition = 5.1 + 3.1;	// 8.2
    double subtraction = 5.1 - 3;	// 2.1
    double multiplication = 5 * 3.1;	// 15.5
    double division = 5.0 / 3.0;	// 1.666...
    ```

* **Comparison operators** (< <= > >= == !=) for integers and doubles
with automatic expansion to double type.
    ```
    bool lt = 5 < 3;		// false
    bool gt = 5 > 3;		// true
    bool le = 5 <= 3;		// false
    bool ge = 5 >= 3;	// true
    bool eq = 5 == 3;	// false
    bool ne = 5 != 3;	// true

    bool doubles = 3.1 > 3.0;	// true
    bool double_int = 3.1 > 3;	// true
    bool int_double = 3 > 3.5;	// false
    ```

* **boolean operators** (&& || !) for booleans
    ```
    bool and = true && false;	// false
    bool or = true || false;	// true
    bool neg = !true;	// false
    ```
* **unary operators** (-- ++) for integer and double variables
    ```
    int i = 4;
    double d = 3.3;

    i++;	// i == 5
    d++;	// d == 4.3
    ```

#### Control flow statements

* **if statement**
    ```
    if (value > 10) {
        value = 10;
    } else {
        if (value < 0) {
            value = 0;
        }
    }
    ```
* **while statement**
    ```
    while (node != null) {
        node = node.parent;
    }
    ```
* **for statement**
    ```
    for (i = 0; i < node.value; i++) {
        println i
    }
    ```

_Note: All flow statements must be defined with curly braces, not doing so
produces syntax error._

#### Parametrized functions
Programmer can define functions with zero or more parameters.
```
int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}
```
When a function is called, its parameter count and parameter types must
match, otherwise a warning is produced.
```
max(2, 5);	// 5
max();	// error: expected 2 arguments, got 0
max(1, 2, 3);	// error: expected 2 arguments, got 3
max(1, 2.0);	// error: expected double type argument
```

### Scope
Declaring two variables (or functions) with a same name in the same scope
produces a warning and the later declaration is not executed.
```
int a = 4;
int a = 6;	// error: redeclaring variable a

double magic() { return 3.14159; }
double magic() { return 2.71828; }	// error: redeclaring function magic
```

Programmer still can declare variables (or functions) with the same
name in different scopes. In such case, whenever a variable (or a
function) is referenced, the variable (or function) defined as
inner-most to the current scope is retrieved and used.
```
int total = 0;
void add() {
    total++;	// increment the global variable
    int total = 10;	// OK, define new local variable, shadowing the global one
    total++;	// increment the local variable
    println total;	// always prints 11
}
add();
add();
println total;	// prints 2
```

#### Local variables and functions
As in Java, programmer can declare local variables inside of functions.
On the contrary to Java, it is also possible to
**define local functions within other functions**. The same way as
local variables cease to exist when the surrounding function returns,
the inner function declaration ceases to exist when the outer function
returns.
```
int max(int a, int b) {
    if (a > b) { return a; } else { return b; }
}
void treeHeight(Tree tree) {
    void nodeHeight(Node node) {
        if (node == null) {
            return 0;
        } else {
            return max(nodeHeight(node.left), nodeHeight(node.right)) + 1;
        }
    }
    if (tree == null) {
        return -1;
    } else {
        return nodeHeight(tree.root);
    }
}
max(2, 3);		// OK
treeHeight(tree);	// OK
nodeHeight(tree.root);	// undefined function
```

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

### How to compile
Crackle uses Flex scanner and Bison parser tools to facilitate the
lexical and syntax analysis.
Crackle is compiled using CMake. CMake handles Flex and Bison
dependencies by itself and one does not have to provide the binaries.
CMake file is a part of src code.

### To be implemented

* Comments
* Function reference as a type -> higher order functions
* Arrays
* Crash nicely when OOM
* Fix leaks
* Unit tests
* Rename object to record (https://en.wikipedia.org/wiki/Record_(computer_science))