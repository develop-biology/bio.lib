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
8. organism (binary interface) - provides a means of running Biology code.

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
::bio::State myState = ::bio::state::Enabled();
```

To recap, extendable types should follow these rules:
* Type is defined in bio namespace. <br />
* Type is UpperCamelCase. <br />
* Corresponding namespace is defined in bio namespace. <br />
* Corresponding namespace is lower_snake_case. <br />

### Templates

Templates are handled in a WYSIWYG manner where `T` (or the appropriate `typename`) is never modified. This means if you call `Add < T >` you will get a `T` returned, not a `T*` or anything else.

### Inheritance & Naming

All Biology classes, with few exceptions, are virtual and may be extended as far as you would like. To accommodate this, objects are almost always passed by pointer; meaning, you can pass references to your derived classes and functions will treat them as the base class. Your code is then executed through virtual method calls.  

`AbstractClasses` cannot be instantiated but can be used to pass implementations by pointer.  

`Classes` may be templated if they necessarily require a template. Otherwise, they contain the minimal functionality they represent and may be extended. 

`ClassOf<>` will be used if `Class` is functional on its own, and we also want a templated derivation. The template (i.e. `ClassOf<>`) will share the same name as the base `Class` with the addition of "of" in order to avoid file name conflicts. It will NEVER be the case that a `Class` derives from a templated base class of the same name (i.e. `Class : ClassOf<>` is invalid as are `Class<> : Class` and `Class : Class<>`). Use of `ClassOf<>` is discouraged; a more descriptive name is preferred (i.e. what does the template override provide?).

Thus, in terms of possible templates, we get either `Class<>` on its own, `Class<> : AbstractClass`, or `ClassOf<> : Class`, depending on whether the base class is functional (of course, we also have `Class : OtherClass<>` but that's just general inheritance).

## Building

### Code

This project depends on the [eons Basic Build System](https://github.com/eons-dev/bin_ebbs), which can be installed with `pip install ebbs`.  
With that said, this is just c++ code. Build it however you want.

Build using ebbs:
```
ebbs
```

installation instructions coming soon (need package manager / repository semantics for module delivery)

### Docs

Documentation is available [on the Develop Biology website](https://develop.bio/doc/libbio/index.html)

## Branding

### Colors
* Dark Blue: #5ca3ff rgb(92, 163, 255) <br />
* Cyan: #13f4ef rgb(19, 244, 239) <br />
* Green: #68ff00 rgb(104, 255, 0) <br />
* Yellow: #faff00 rgb(250, 255, 0) <br />
* Orange: #ffbf00 rgb(255, 191, 0) <br />
* Magenta: #ff5794 rgb(255, 87, 148) <br />
