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


# Native Biology Code

The bio library provides the basics for creating a directed, recursive hypergraph by allowing, among other mechanisms, the ability of objects to bond other bonded objects and allowing neurons to communicate through synapses, which are themselves bonded objects (i.e. synapses may use or even be neurons and vice versa). Thus, our graphical programming system blurs the lines of what it means "to be", "to have", and "to do".  From this abstract vantage point, we may rethink what it means for a language to be "functional" vs "object oriented" and much more. However, this framework is only as powerful as what we allow in and out of it. Thus becomes paramount, the feature of symmetry which allows arbitrary data structures, including functions, to be reflected into and out of other systems. These other systems (properly defined as Axes) may be specialized function calls or entire other programming languages. We will now turn our focus to the language we have developed to wrap this library's abstract graph: Native Biology Code.

## Blocks
Native Biology Code is almost like an object-oriented lisp variant and relies heavily on the meaning of different braces. The braces used are:
* `()` - Surfaces & Execution (i.e. API)
* `[]` - Internals & Solutes (i.e. members)
* `{}` - Execution Definitions (i.e. functions)
* `<>` - Template Programming through Transcription Factors (i.e. types)
* `""` - Strings
* `/**/` - Comments & Documentation


## Types
Different data structures are defined by which blocks they possess. At minimum, a definition must have 2 or more coding (i.e. non-string, non-comment) blocks.
The available data structures are as follows; "symbol" represents the name of whatever the expression defines:
* `symbol type` - predefined type
* `symbol()[]` - Vesicle (i.e. struct)
* `symbol()[]{}` - Protein (i.e. function)
* `symbol<>()[]{}` - Cellular Structure, such as a Neuron, Tissue, or Organ (i.e. class)
* `symbol<>{}` - Transcription Factor (i.e. decorator, parent class, meta programming)
* `symbol<>[](symbol()[]{})` - Plasmid + RNA Polymerase (i.e. a package)
* `symbol<>[]{}{}` - Synapse, with an upstream and downstream function (i.e. edge)

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
There is no inheritance in Native Biology Code. This might be surprising considering how much inheritance is (ab)used in the C++ code underpinning the language. We believe a simple meta-programming along with native code rearrangement is cleaner and less error-prone.

Transcription Factors may perform arbitrary changes to the data structures they are applied to.
For example:
```
symbol <dependencies> 
{
    object.newSurface string;
    object.internal.newVar int;
    object.execution.add(someExecutionSatement(newVar, newSurface));
}
```
TFs use the same kind magic variables that Synapse uses: our `object` here. The `internal`
## Casting


## Execution
If an expression has 1 or fewer blocks, it will be executed.
