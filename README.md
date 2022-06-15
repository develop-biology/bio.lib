# Develop Biology
__Programming Life__

![build](https://github.com/develop-biology/lib_bio/actions/workflows/ebbs-pr.yml/badge.svg)


**THIS CODE IS PRE-ALPHA**
We'll be releasing the alpha version as soon as we can!

As a part of eons, we follow all [eons conventions](https://eons.llc/convention/).

## Style

The purpose of our coding style is to make wrong code look wrong. If something seems out of place, then it probably is. We treat style as another tool to help us write better code faster.

Here are some of the rules we follow:
1. Variables follow lowerCamelCase.
2. Variables begin with a lowercase letter.
3. Variables will never contain underscores.
4. [Hungarian notation](https://en.wikipedia.org/wiki/Hungarian_notation) is reserved for variables of possibly obscure scope. For example member variables (m...) and global variables (e.g. s... for static) which are not system-wide should be decorated in a way that provides the reader with a hint to where the variable came from. If a variable is not decorated it should be a part of the local scope or, in rare cases, a system-wide global variable which is not part of our code base.
5. Functions follow UpperCamelCase.
6. Functions begin with a capital letter.
7. Functions never contain underscores.
8. Functions do not currently use Hungarian notation but there is no good reason they shouldn't.
9. Macros are ALL_CAPS_WITH_UNDERSCORES.
10. Macros will never have a lowercase letter.
11. Namespaces are all_lowercase_with_underscores.
12. Namespaces will never contain a capital letter.
13. Classes, Structs, Enums, etc. all follow UpperSnakeCase. This means a Function() and a ClassInstantiation() will look the same but both will operate the same (i.e. returning an object of the indicated type).


A few notable implications of this:
1. Any &UpperSnakeCase will always be the address of a function because you cannot take the address of a class.
2. Classes:: and namespaces:: will always be visually distinct.

### Namespaces
Biology is broken into the following namespaces:
1. common (base) - the corollary to real-life mathematics; used everywhere.
2. physical (nodes) - adds relativity, base classes, & real-life type simulacra.
3. log (logging) - logs
4. chemical (edges) - expands on physical base classes, adding bonds, reactions, and structure.
5. molecular (values) - adds symmetry and context to chemical base classes.
6. genetic (modules) - adds the ability to combine molecules & proteins into discrete packages.
7. cellular (library interface) - combines all lower namespaces and provides common points for library extension.
8. api (simplified types) - removes extensibility from types, allowing for easy use of custom classes.
9. organism (binary interface) - provides a means of running Biology code.

#### Extension
In order to facilitate cross-namespace type extension, some special rules should be followed in order to create consistency between domains.

When defining extendable types, primarily through the use of `physical::Perspectives`, it is customary to define the type with a capital (i.e. Type) in the main bio namespace, ignoring any other namespaces the type would otherwise belong to.
The definitions of type-returning functions or raw types should then be placed in a lowercase namespace; again, this namespace should be directly under the main bio namespace and not in whatever sub-namespace the type would otherwise belong to.
For example, States are defined in the physical folder but not in the physical namespace. They are then defined in the `::bio::state` namespace.
This snippet is from "bio/chemical/States.h"
```c++
namespace bio {
namespace state { //<- lowercase "state", the namespace

State Enabled(); //<- Capitalized "State", the type (::bio::State, not ::bio::state::State).

} //state namespace
} //bio namespace
```
So, you can say
```c++
bio::State myState = bio::state::Enabled();
```

To recap, extendable types should follow these rules:
* Type is defined in bio namespace.
* Type is UpperCamelCase.
* Corresponding namespace is defined in bio namespace.
* Corresponding namespace is lower_snake_case.

### Inheritance

#### What you need to know
1. The order of inheritance matters (see common/VirtualBase.h for why).
2. Classes should always derive FIRST from a `Class< T >` class using [CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern).
3. Ambiguous virtual methods should be resolved with `BIO_DISAMBIGUATE_ALL_CLASS_METHODS(...)` or manually by selectively pointing to the `Class< T >` implementations (see physical/macros/ClassMethodMacros.h).
4. Simple constructors may be optionally defined with `BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(...)` for extensible types or `BIO_CONSTRUCTORS(...)` for `api` types (see chemical/macros/ConstructorMacros.h and api/macros/ConstructorMacros.h).
5. If using `api` classes, you will NOT be able to combine objects by inheriting from them and must `Bond` what you need in a `CommonConstructor()` or equivalent method (see chemical/Atom.h for how to form Bonds).

#### Design
In real-world chemistry, the atoms a substance has determine what it is and there is no difference between a "has a" and "is a" relationship. Such is also true of many other fields, making the concepts of [composition and inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance) moot. In order to align our code with the real world, Biology leans into object orientation and C++ complexities pretty hard. We go deep enough into inheritance world that we derive the ability to inherit through composition via `chemical::Bonds`. This kind of "horizontal" or even "runtime" inheritance is a simple mimic of the compiler machinery that makes inheritance possible in the first place. So, like the rest of our framework we try not to reinvent the wheel but, instead, make many gears fit together. By learning about [vtables and how inheritance is often implemented](https://en.wikipedia.org/wiki/Virtual_method_table) or about how [chemical bonds](https://en.wikipedia.org/wiki/Chemical_bond) determine the properties of a substance, you should be able to better understand the Biology Bonding interface.

### Templates

Templates are always treated in a bare-minimum, unqualified type fashion. Meaning that you might say `Get< MyClass >()` and be given a `MyClass*`. This is confusing. However, it allows maintenance on the code without breaking downstream APIs. For example, in a later version, you might get a `std::smart_ptr< MyClass >`. You should be able to rely on the documentation to see what is taken and returned in a templated symbol.

A few notes:
1. Any class which derives from `bio::physical::Wave` should be treated as a pointer type in all user-facing routines. This means that if you use or derive from a Biology class, you will be using a pointer type.
2. The inverse of the above also holds true: all built-in types and classes which do not derive from `Wave` should be treated as non-pointer types, or as pointers to a`bio::physical::Quantum< T >` (which should be implicitly usable as a `T`).

These rules also apply to preprocessor macros as much as possible.

### Multithreading

If you're not using multiple threads, simply compile with `BIO_THREAD_ENFORCEMENT_LEVEL 0` and ignore the rest of this section.
Otherwise, please compile with as high of a `BIO_THREAD_ENFORCEMENT_LEVEL` as you can. The current max is 2 but 10 would work just fine.

All classes that have data which needs to be protected derive from `ThreadSafe`. Just because a class derives from `ThreadSafe` does not mean you can do whatever you want with it, and it will never break, unfortunately.

In order to use a `ThreadSafe` class safely, you must use `SafelyAccess` to create an external lock. For example:
```cpp
SafelyAccess< MyPerspectiveSingleton >()->GetIdFromName("Some Name");
```
or
```cpp
MyType* mine = GetMineSomehow();
SafelyAccess< MyType >(mine)->CallMyMethodSafely(mine->NowSafeAccessToAnotherMethod());
```
Note that in the last example we only use SafelyAccess once while making two calls to `mine`. This is how the `SafelyAccess` [RAII](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization) mechanism must work. You cannot call `SafelyAccess` on the same object in the same thread twice: doing so is an error which is subject to the `BIO_THREAD_ENFORCEMENT_LEVEL` (i.e. you code will crash at high enforcement levels; this is what you want).

To recap, Biology handles threading by providing each resource with the option to be locked independently. Locking itself is then done through the RAII external locking mechanism provided by `SafelyAccess`.

## Building

### Code

This project depends on the [eons Basic Build System](https://github.com/eons-dev/bin_ebbs), which can be installed with `pip install ebbs`.  
With that said, this is just c++ code. Build it however you want.

Build using ebbs:
```
cd build
ebbs -c build_local.json
```

installation instructions coming soon (need package manager / repository semantics for module delivery)

### Docs

Documentation is available [on the Develop Biology website](https://develop.bio/doc/libbio/index.html)

## Branding

### Colors
* Dark Blue: #5ca3ff rgb(92, 163, 255)
* Cyan: #13f4ef rgb(19, 244, 239)
* Green: #68ff00 rgb(104, 255, 0)
* Yellow: #faff00 rgb(250, 255, 0)
* Orange: #ffbf00 rgb(255, 191, 0)
* Magenta: #ff5794 rgb(255, 87, 148)

--------------------------------------------------------------------------------

# Native Biology Code

The bio library provides the basics for creating a directed, recursive hypergraph by allowing, among other mechanisms, the ability of objects to bond other bonded objects and allowing neurons to communicate through synapses, which are themselves bonded objects (i.e. synapses may use or even be neurons and vice versa). Thus, our graphical programming system blurs the lines of what it means "to be", "to have", and "to do".  From this abstract vantage point, we may rethink what it means for a language to be "functional" vs "object oriented" and much more. However, this framework is only as powerful as what we allow in and out of it. Thus becomes paramount, the feature of symmetry which allows arbitrary data structures, including functions, to be reflected into and out of other systems. These other systems (properly defined as Axes) may be specialized function calls or entire other programming languages. We will now turn our focus to the language we have developed to wrap this library's abstract graph: Native Biology Code.

## Blocks
Native Biology Code is almost like an object-oriented lisp variant and relies heavily on the meaning of different braces. The braces used are:
* `()` - Surfaces & Execution (i.e. API)
* `[]` - Internals (i.e. private members)
* `{}` - Execution Definitions (i.e. functions)
* `<>` - Template Programming through Transcription Factors (i.e. types)
* `""` - Strings
* `/**/` - Comments & Documentation


## Types
Different data structures are defined by which blocks they possess. At minimum, a definition must have 2 or more coding (i.e. non-string, non-comment) blocks.
The available data structures are as follows; "symbol" represents the name of whatever the expression defines:
* `symbol type` - predefined type
* `symbol()[]{}` - Protein (i.e. function or synchronous functor)
* `symbol<>()` - Vesicle (i.e. array or struct)
* `symbol<>()[]{}` - Cellular Structure, such as a Neuron, Tissue, or Organ (i.e. class, node, or stateful asynchronous function)
* `symbol<>[]{}{}` - Synapse, with an upstream and downstream function (i.e. edge)
* `symbol<>{}` - Transcription Factor (i.e. decorator, parent class, meta programming)
* `symbol<>(){}` - Plasmid + RNA Polymerase (i.e. a package)

There are no differences between arrays / lists, maps / dictionaries, sets, etc. In Native Biology Code, items in a Vesicle may be mixed and match to your liking.

If you do not know the type you want ahead of time, you can use `*` in the type declaration and set the type and value at once with "=". This is similar to the `auto` keyword in C++.

`*` is also used after a type to indicate copy-by-value.
`**` means any time, copied by value.

## Control Flow
There are only 2 forms of control flow available in Native Biology Code: the `if` statement and the `while` loop + break statement.

Control flow statements must be an entire expression and cannot be part of a larger expression. This means they must begin with either `{` or `;` and end with either `;` or `}`.

`if` expressions are defined as `...?{}` and `if...else` expressions are defined as `...?{}{}` where `...` is the condition to check.

`while` loops are defined as `(...){}`, where `...` is the condition to run the loop again.
Break statements are simply `break`.

Break statements return execution of any `{}` to the caller. Meaning, they may be used to stop regular function calls from running to completion.

## Syntax
If an expression has 1 or fewer coding blocks, it will be executed (e.g. `functionCall()` or `functionCall` vs `functionDeclaration()[]{}`).
All executed statements (i.e. those within a `{}`) must be separated by (i.e. end with) a `;`.
All items in a `()`, `[]`, or `<>` block must be separated by a `,`.

It is an error to redefine a symbol. `type type` will always be an error. `symbol symbol` is an error when used in isolation, as it will be interpreted as a redefinition of `symbol` to the type which `symbol` defines.

However, `symbol symbol` may be valid when used as a part of a larger, executed expression. These rules are applied depending on the types of the left-hand statement (LHS) and the right-hand statement (RHS):
* If LHS is a Protein and RHS is a Vesicle or Cellular Structure, a wrapping `()` is applied to the RHS.
* If LHS is a Protein and RHS is also a Protein which is defined as a Surface on the LHS, a `.` is added to replace the whitespace between the symbols.
* If LHS is a Protein and RHS is also a Protein which takes at least 1 argument and is not defined on the Surface of the LHS, the LHS will be type-casted to the type of the first argument of RHS (e.g. `add(1,2) > 3`). This may result in an error if the type-casted LHS does not implement RHS on its Surface.
* It is an error if LHS is a Protein and RHS is also a Protein which takes no arguments and is not defined on the Surface of the LHS.
* If the LHS is a Vesicle or Cellular Structure and the RHS is a Protein, Vesicle, or Cellular Structure, a `.` is added to replace the whitespace between the symbols.

Thus, consecutive symbols become expressions in a right to left manner to automatically fill in function calls and member access. This means we can define `+` as a custom function in an object and say `a + b`, which becomes `a.+(b)`. It is an error if the LHS is a Vesicle or Cellular Structure and does not define the given symbol or if the LHS is a Protein which does not have a Surface of the RHS type and the RHS cannot be converted to a type the LHS takes (see below for more on type conversion).

You cannot declare variables in an execution block (`{}`).  
You may execute expressions in declaration blocks (`()` and `[]`) and type blocks (`<>`), provided that they:
* include single-block expressions (i.e. `functionCalls()` or `indexAccess[]`) and 
* do not contain spaces (i.e. none of the `symbol symbol` autofill logic is enabled in declaration blocks).

There is no `this`, `self`, etc. all unprefaced symbols in an execution block are prefaced with the current symbol to which the execution block belongs. If no such symbol can be found, the symbol which defines the containing execution block will be searched and so on until all contexts are exhausted. It is an error if a symbol is not found in any execution block leading to the current expression.

Members defined within an object's `[]` will only be searched within that object's `{}`. No other object within or without shall have access to an object's internal members. The only exception to this is Transcription Factors, which have access to any `[]` they can reach.

It is an error if a member is declared in both a `()` and `[]`. The same symbol can be used in a `<>` as in either `()` or `[]`, since the meaning of `<>` is dependent on the object to which it applies and will never be used for member definition.

Surfaces may be accessed by their name or simply by their index using the `[]` operator in an executable expression.

Any symbols which begin with `__` (2 underscores) are compiler defined and may not be changed. It is illegal for your variables to begin with underscores.

Proteins (i.e. functions, or more accurately functors, or even more accurately `()[]{}`) do not have return types. This is to enable multiple and delayed access to returned values.
If you would like to treat a Protein as a type, you may define a type conversion for the Protein, which will make the syntax appear as a return value in other languages. There's more on that below.

## Copy by Reference by Default
Objects are normally copied by reference and any change to the object will be changed in all locations.
These changes are thread-safe by default and may wait on other threads.

However, if you would like to copy an object by value (possibly for non-blocking or destructive access to the object), indicate such in the type declaration.
For example:
```
add(first int*, second int*, result int)[]
{
    result = first + second;
}
```
Here, `first` and `second` are copied by value and will remain a part of the `add` Protein until they are unbound.

## Garbage Collection
When an object has no references, it is deleted. This is handled through the `chemical::` `solution` system in the underlying C++.

## File Inclusion

`&"/path/to/file.bio"` will include all the contents of the specified file in the exact location the `&` statement occurs in.

This means you can do crazy things like create a file with the contents:  
`"FunctionDefinition.bio"`
```
doStuff(with, internals)
```
And then use that file in your code:
`"Main.bio"`
```
MyFunction(with int)[internals <>()]
{
    &"FunctionDefinitions.bio"
}
```

## Asynchronous Cellular Structures and Event Dispatch
Most execution blocks, such as Proteins, Transcription Factors, and Plasmids run synchronously and immediately in the thread in which they are called.

All Cellular Structures (i.e. `<>()[]{}`) execute their `{}` on a clock. The speed of their oscillation may be set with the built-in value `interval` in milliseconds (e.g. `MyCell<>()[]{interval ++;}`, which would make `MyCell` call its `{}` 1ms slower every time it's called). Cellular Structures do not always run in separate threads. It may be useful to have a Cellular Structure, like a tissue, contain other Cellular Structures and have their oscillations run in a single thread. Other times you may want maximum parallelism which can be accomplished by calling the built-in `fork()` function. All `fork()` does is move the Cellular Structure to a new thread. Note that you can just say `MyCell fork;`, which will expand to `MyCell.fork();`

## Simple Neuron Example
An example of a Neuron could be:
```
MyNeuron /*A Neuron with gain control*/ <neuron>
(input int, output int) [gain int = 1]
{
    output = gain * input;
}
```
Here, we start the definition of `MyNeuron` with a comment block (`/**/`). This acts as the documentation for the object. Note that newline characters are only ever interpreted in strings and comment blocks, so you can have whatever fancy documentation you'd like. Other than strings and comments, newlines and all other whitespace is mostly irrelevant (more on that later). This allows us to put spaces and even newlines in our blocks.  
Next, we specify the `neuron` Transcription Factor (TF). This tf will make what would otherwise be a `Cell` (`symbol<>()[]{}`) into a `Neuron` with the ability to receive and send signals.   
We then add 2 publicly accessible members, `input int` and `output int`. These are built-in `int` types but could be whatever we want. After defining our publicly available Surfaces, we define a private member `gain int` and set the default value to 1. Because `gain` is in square brackets (`[]`), it will not be accessible anywhere outside the execution block (`{}`) of our `MyNeuron`, except in rare cases (such as with TFs, more on that later).  
Lastly, we define the execution block on a new line and with indents. However, the indentation and white space do not matter. Notice the line ending used in the execution block: `;`. These line endings are only needed to separate executed expressions from each other and are unnecessary when defining symbols, hence no line ending after we complete our `MyNeuron` definition.

## Simple Synapse Example
Now, let's create a Synapse to connect to our Neuron:
```
MySynapse <synapse>
/**
 * This synapse moves the outputs of one MyNeuron into the inputs of another. 
 */
[
    passthrough int
]
{
    //upstream is a magic value that is provided in the first execution block of a Synapse.
    passthrough = upstream.output;
}
{
    //downstream is a similar magic value provided in the last execution block.
    downstream.input = passthrough;
}
```
We've done a few things differently here. Notice that we changed the position of the comment block to be after the TFs. This is valid; blocks may appear in any order. The caveat to that is that Synapses have duplicated blocks and the first one to appear is always provided the `upstream` value containing the source Neuron while the last is always provided the `downstream` value containing the target neuron.

Notice also that we use the `.` for member access and that we are accessing Surfaces of the provided variables. All variables are passed by reference and the `.` is the only member access specifier.

We can now use this Synapse with our connection notation: `SourceNeuron --- Synapse ---> TargetNeuron`.
Here's how that could look:
```
source MyNeuron --- MySynapse ---> target MyNeuron
```
Alternatively, we can declare our variables ahead of time:
```
source MyNeuron
target MyNeuron
synapse MySynapse
source --- synapse ---> target
```
Alternatively, if we wanted to define our Synapse only using TFs, we could:
```
source MyNeuron
---
[]{}{}
<
    all,
    of,
    these,
    //but,
    //not these
    transcription,
    factors
>
--->
target MyNeuron
```
Here, the Synapse declaration is a Type Expression.

## Type Expressions
Type Expressions can be substituted for full types the same way defined symbols can be treated as types.
Saying `var MyNeuron` is just as valid as saying `var <neuron>(input int, output int) [gain int = 1]{output = gain * input;}`. These expressions are in fact identical. The only time you need to explicitly define the symbol name is if you are going to use it later.
Consider the above example:
```
source MyNeuron --- MySynapse ---> target MyNeuron
```
Since we have no need to modify `MySynapse` after we create it, we do not have to give the `MySynapse` type a name.

## Meta Programming and Inheritance
There is no inheritance in Native Biology Code. This might be surprising considering how much inheritance is (ab)used in the C++ code underpinning the language. We believe it is cleaner and less error-prone to use a simple meta-programming along with native code rearrangement through self-reflection.

Transcription Factors may perform arbitrary changes to the data structures they are applied to.
For example:
```
symbol <dependencies> 
{
    object.newSurface string;
    object.[].newVar int;
    object.existingSurface.[].internalValue = 4;
    object.{} += someExecutionStatement(newVar, newSurface);
    object.{}.someOtherFunction = (same type, of args)[but with, also these]{and(same, but, also, "modified functionality");};
}
```
TFs use the same kind of magic variables that Synapse uses: our `object` here is provided to a TF so that it may affect whatever it is applied to.
TFs have access to all parts of all objects which may be reached through this provided `object`. This is accomplished through the `genetic::` `Localization` and `Insertion` mechanics. 
In this example, `object.newSurface string;` creates a new `Surface` (i.e. `()`) called `newSurface` which is of type `string`. We don't need to specify `object.().newSurface`, as we do with the internal (i.e. `[]`) variable `newVar`. This is because members are exposed (i.e. public) by default and by specifying `object.[]`  only when making members internal (i.e. private), we save ourselves some work.
`object.existingSurface.[].internalValue = 4;` shows how TFs have the ability to modify ANY internal value they can access, not just those of `object`.
The line `object.{} += someExecutionStatement(newVar, newSurface);` appends a new execution statement to whatever object already had in its execution block (i.e. `{}`).
The last line of this example uses the syntax `object.{}.someOtherFunction ...` which addresses all instances of `someOtherFunction` within the object's execution block. This does not affect `someOtherFunction` in any other object.

You are also allowed to modify comments and strings through these same mechanics. `object./**/ += "New documentation";` is valid.

### Composition
The easiest way to reuse the code you've written is through composition. Let's take the `someOtherFunction` example from above and assume you wrote:
```
MyModifiedFunction
(
    same type,
    of args
)
[
    but with,
    also these
]
{
    and(same, but, also, "modified functionality");
}
```
You could then create a TF wrapper to enable code reuse with the following:
```
modifyFunctionality <>
{
    object.{}.someOtherFunction = MyModifiedFunction;
}
```
BAM! Done. Now you can `<modifyFunctionality>` wherever you want!

### Inheritance
A traditional inheritance style Transcription Factor interface requires a bit more work to build.
Consider:
```
Animal<>(legs int = 0, makeNoise ()[]{print("NOT IMPLEMENTED";})[]{} //pretend print()[]{} was defined beforehand  
Cat<>(legs int = 4, makeNoise ()[]{print("Meow";})[]{}
```
While you can use `Cat` anywhere you can use `Animal` (and vice versa, in this case), making `Cat` effectively an `Animal`, we shouldn't be duplicating code.
Let's simplify this:
```
AnmialTF<>
{
    object.legs int = 0;
    object.noise string = "NOT IMPLEMENTED";
    object.makeNoise = ()[]{print(noise);};
}
CatTF< AnimalTF >
{
    object.legs = 4;
    object.noise = "Meow";
}

cat < CatTF > ()[]{}
cat.makeNoise();
```
Now, we can define any number of Animals through Transcription Factor dependencies and any change in the `AnimalTF` "base class" will be applied to all Animals.

### Meta Meta
Let's talk a little bit about how the above inheritance system actually works. When a Transcription Factor has a dependency, that dependency is loaded before the execution block. Thus, the above example of:
```
CatTF< AnimalTF >
{
    object.legs = 4;
    object.noise = "Meow";
}
```
expands to:
```
CatTF<>
{
    //From AnimalTF
    object.legs int = 0;
    object.noise string = "NOT IMPLEMENTED";
    object.makeNoise = ()[]{print(noise);};
    
    //From CatTF
    object.legs = 4;
    object.noise = "Meow";
}
```

We could further expand on this with:
```
KittenTF<CatTF>{object.noise = "mew";}
```
which would expand to:
```
KittenTF<>
{
    //From AnimalTF
    object.legs int = 0;
    object.noise string = "NOT IMPLEMENTED";
    object.makeNoise = ()[]{print(noise);};
    
    //From CatTF
    object.legs = 4;
    object.noise = "Meow";
    
    //From KittenTF
    object.noise = "mew";
}
```

This code is executed in order, as expected. No tricks.
If we redefine `AnimalTF` to end with the following line: `object.makeNoise()` and write:
```
kitten <KittenTF>()[]{}
kitten.makeNoise();
```
We would get:
```
print("NOT IMPLEMENTED);
print("mew);
```
Since the expanded code would be:
```
KittenTF<>
{
    //From AnimalTF
    object.legs int = 0;
    object.noise string = "NOT IMPLEMENTED";
    object.makeNoise = ()[]{print(noise);};
    object.makeNoise(); // <--- THE LINE WE ADDED
    
    //From CatTF
    object.legs = 4;
    object.noise = "Meow";
    
    //From KittenTF
    object.noise = "mew";
}
kitten <KittenTF>()[]{}
kitten.makeNoise();
```
We will come back to this example after we expand on how execution happens.

## Initialization
There are several means by which you might create a constructor. Destructors do not exist in Native Biology Code but can likewise be accomplished by having a caller call some destructor method.

The easiest way to create a constructor is simply define a TF for your class.
For example, we can break the above example of `AnimalTF`'s call to `makeNoise()` into 2 TFs: a static initialization and functional initialization:
```
Anmial<>
{
    object.legs int = 0;
    object.noise string = "NOT IMPLEMENTED";
    object.makeNoise = ()[]{print(noise);};
}
AnmialConstructor<>
{
    object.makeNoise();
}

Cat< Animal >
{ 
    object.legs = 4;
    object.noise = "Meow";
}
CatConstructor< AnimalConstructor > {}

Cat< Animal >
{ 
    object.legs = 4;
    object.noise = "Meow";
}
CatConstructor< AnimalConstructor > {}

Kitten< Cat >
{ 
    object.noise = "mew";
}
KittenConstructor< CatConstructor > {}
```
Now, `kitten < Kitten, KittenConstructor >()[]{}` expands to:
```
kitten
<
    //From Animal
    object.legs int = 0;
    object.noise string = "NOT IMPLEMENTED";
    object.makeNoise = ()[]{print(noise);};
    
    //From Cat
    object.legs = 4;
    object.noise = "Meow";
    
    //From Kitten
    object.noise = "mew";
    
    //From AnimalConstructor
   object.makeNoise();
   
   //From CatConstructor
   
   //From KittenConstructor
>()[]{}
```
Which prints the expected "mew".

Another way we can create a constructor or destructor is through a Surface definition.
We can say:
```
kitten < Kitten >
(
    constructor ()[]
    {
        makeNoise()
    },
    destructor ()[]
    {
        explode()
    }
)
[
    explode ()[]
    {
        print("BOOM!");
    }
]
{}

kitten.consturctor();
kitten.destructor();
```
Now, the caller can construct and destruct our `kitten`.

### Meta Initialization
While all variables must be declared on the Surface or internally before they are used in an execution block, you can use execution statements to declare variables.
This is valid:
```
MyCell<>
(
    GetSurfaceFor("MyCell").name GetSurfaceFor("MyCell").type = GetSurfaceFor("MyCell").value  
)
[]
{}
```

Note that we cannot include spaces in expressions used in declaration blocks, so `GetSurfaceFor "MyCell" GetSurfaceFor "MyCell" = GetSurfaceFor "MyCell"` is invalid and will generate an error.

## Type Conversion and Casting
A type may be treated as another type if it defines the target type on its Surface.
For example:
```
InventoryNumber<>
(
    int trakingId
)
[
    trackingId int
]
{}
```
When a function like `1 + InventoryNumber` is called, the expression is interpreted as `1.+(InventoryNumber.int)`, since `1`, being an `int` expects an `int` in its `+` method. `InventoryNumber.int` then produces `InventoryNumber.trackingId`.
This works for any type.
```
Package< SomePackageImplementation >[](){}

TrackPackage(trackingId int, result Position)[track SomePackageTrackingImplementation]{result = track(trackingId).result;}

GetPackage(Position where, result Package)[get SomePositionLookupImplementation]{result = get(position).result;}

InventoryNumber<>
(
    int trakingId,
    Position TrackPackage(trackingId).position,
    Package GetPackage(Position).result
)
[
    trackingId int
]
{}
```
Now, our `InventoryNumber` can be treated directly as a `Position` and even passed in place of a `TrackPackage` function call (i.e. `InventoryNumber(someInt).result` discards the provided int and provides its own `Position` as the `result`).
Note that `Position`, when used in the type expression defining `Package` references the `Position` Surface of `InventoryNumber` and not the `Position` type. If an object does not define a symbol and that symbol can't be found on the Surfaces of containing objects, it is an error. Thus, we cannot say `Package GetPackage(GetPackage).result` because `InventoryType` cannot be treated as a `GetPackage` Protein.

## Function Arguments
Arguments may be provided by order or by symbol specification. For example, consider:
```
add(first int, second int, result int)[]{result = first + second;}
three int = add(1, 2).result;
three int = add(1, second = 2);
three int; add(1, 2, three);
three int; add(1, 2, result = three);
```
All of these are valid ways to use the `add` Protein.

Note that we can treat `add` as if it had a return value with:
```
add
(
    first int,
    second int,
    result int,
    
    //return value
    int result
)
[]
{
    result = first + second;
}
```
Now, we can say `three int = add(1, 2)`, without needing to specify `.result`.

Here's another example:
```
sort(array <>())
[
    index int,
    buffer * //we want buffer to be any type, so we mark it as "*", which allows us to set the type and value later with "=".
]
{
    array size <= 1 ? {break};
    
    //the above statement is dense.
    //array size expands to array.size(), which gives the number of Surfaces in a Vesicle or Cellular Structure (and may be overriden).
    //the rest of the if condition expands to: array.size().<=(int).
    //the Native Biology Code interpreter will check array.size() for an int conversion, which exists, and thus will call: array.size().int.<=(int).
    //the if statement then reads ...int.<=(int).bool 
    //the if statement (i.e. condition ? {true case};) does not require the word "if" nor parenthasese and is terminated with a ";", just like every other expression.
    //because "break" is the last (and only) expression in the if execution block, it does not need to end with a ";".
    //"break" ends the function, returning control to the caller.
    
    index = 0;
    (true) //while loop
    {
        array[index] > array[index+1] ?
        {
            buffer = array[index];
            array[index+1] = array[index];
            array[index+1] = buffer;        
        };
        
        index ++; // note the space after "index", making the expression expand to index.++().
        index == array size ? {break}; 
    };
}

Pair <>
(
    name string,
    value int,
    
    //Type Conversions (for predefined ">" operation)
    int value
)

heterogeneosuMap <>
(
    var1 Pair = ("One", 1),
    var3 Pair = ("Three", 3),
    var4 int = 4,
    var2 Pair = ("Two", 2)
)

sort(heterogeneosuMap);
```
This will result in `heterogeneosuMap` being rearranged as:
```
heterogeneosuMap <>
(
    var1 Pair = ("One", 1),
    var2 Pair = ("Two", 2)
    var3 Pair = ("Three", 3),
    var4 int = 4,
)
```
This works on `heterogeneosuMap` with a mixture of Pairs and int(s) just as it would on any data which can be represented as an array of ints. Note that the int requirement only exists because of the `int.>(int)` call, which only takes ints. If we had a map consisting of custom types which implemented `> (other CustomType)[]{}`, this would work just as well.
