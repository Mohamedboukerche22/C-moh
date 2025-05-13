🌐 C-moh Programming Language in C++

This is a simple interpreter written in C++ for a Python-style English-like language.
It supports basic features such as:

    print statements

    let variable declarations

    Assignments and expressions

    if statements

    while loops

    Python-style indentation for blocks

💻 Example Program
```cpp

let x = 3
print "Starting loop"
while x > 0:
    print "x is"
    print x
    x = x - 1
print "Done"
END
```

🧠 How It Works

 ``` let x = 3 ``` → defines a variable

 ```print``` → prints a message or variable

  ```while x > 0:``` → loops while condition is true

  Indentation defines code blocks (like Python and js)

  ```END``` stops the input

### how to run
```bash
g++ -o c-moh main.cpp
./c-moh
```
