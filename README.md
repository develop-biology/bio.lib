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
* Dark Blue: #5ca3ff rgb(92, 163, 255) <br />
* Cyan: #13f4ef rgb(19, 244, 239) <br />
* Green: #68ff00 rgb(104, 255, 0) <br />
* Yellow: #faff00 rgb(250, 255, 0) <br />
* Orange: #ffbf00 rgb(255, 191, 0) <br />
* Magenta: #ff5794 rgb(255, 87, 148) <br />
