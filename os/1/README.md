## Task 2

- When would you use a sorted binary tree over a linked list?
	When sorted data (sorted by an associated key) is needed for fast lookup/search.

- What would you change if the tree had to support not just integers, but any type of data?
	Instead of the int store a void* for data and a function pointer to a comparison function.
	Or alternatively store a void* for data and a separate key (although, can easily be implemented using the first approach).

- Why do we use modules instead of putting everything a single file?
	When using modules, a module can be written once and used in many projects.
	Also, a module implementation can be changed / swapped with another one without the need to change the using code or even recompiling it.

- Is the development cycle (i.e. changing a source file, recompiling and running tests) faster when using modules? Explain your answer.
	Yes, it is faster.
	Only the changed source file needs to be recompiled and the application linked with the recompiled module, instead of recompiling everything.

	Without using a modular design, testing is very difficult to implement.

- What is a *header guard* and why is it needed?
	A header guard is a macro (or #pragma once) used at the top of a header file to avoid including the file multiple times, which would lead to redeclaration errors.

- Why are `btree` and `btree_node` not *defined* in the header file? What are the implications?
	The implementation of the `btree` and `btree_node` structs is hidden inside of the module implementation.
	It allows the implementor to use an individual struct layout best fitting the implementation.
	And it avoids direct modification of the tree without using the provided methods, like making all members private.
	Also from the user side of the btree, the implementation is not important anyway.

- Explain the `const` in the parameter list of `btree_print`, is it required?
	const tells the compiler, that the passed tree must not be modified inside the function.
	It is not required, but a print function is expected to not modify a tree.
	Although it is required if used on const btree pointers.
	By using the const qualifier it is guaranteed that the tree is not modified by the function (except when using ugly casts).

- Explain memory leaks. Why are memory leaks bad? You should know how to use `valgrind` to check your program for memory leaks.
	A memory leak is, when code is allocating memory but does not free it.
	Depending on how much that particular code is used, the leaked memory will sum up and cause very high memory usage.
	A severe memory leak can also crash the whole system.

- What is the reason behind writing *everything* in English?
	By writing everything in english it is much easier to understand code for everyone, regardless of who wrote it.

- Why should you use `static` for *non-exported* functions?
	By using static for non-exported functions, the symbols are only visible inside the translation unit.
	Thus, they can not collide with functions named the same inside other translation units (modules, etc.).

- Why should we comment our source code? Is it always need? What should the comment state? What is *self-documenting code*?
	Source code should be commented if it is not trivial to help (other people and the coder after a longer period of time) understanding the code when reading it.
	It is not needed for trivial code, whose purpose is obvious anyway.
	The comment should describe what the complicated part in the code does, or sometimes why.
	self-documenting code is accomplished through using variable and function names who describe their purpose.
	So, by reading the names it becomes clear, what the variable/function accomplishes.

- Why should the module not output debug messages?
	A module is meant to be used in any application, and not only by the module author.
	Depending on the application, debug messages will be inappropriate.
	Or the way of outputting debug messages is different to how they are handled in the particular application.

	Anyway, a module should be thouroughly tested so debug messages are not needed when using it.

- Why and when should you use `assert`?
	assert should be used to test for invalid situations, which make the application unable to work normally.
	Like calling a function with a NULL pointer, where the pointer must not be NULL by definition.
	This case would typically be handled by an assert.

- What are the benefits of using `make` over calling the compiler by hand?
	By using make, all relevant compiler options, compiler calls etc. are configured in a global file and need not be remembered or known by the person who compiles the code.
	Also, make constructs a dependency model of the project and does automatically only compile changed files and their dependents.

- Imagine it was your job to design the interface for the btree module (writing `btree.h`). What would you have done differently, and why?
	I would not declare a btree_node struct. It should be the implementers duty, since no function working on btree_node is exported and thus can not be used outside of the module.
	The description of the btree_print function is a bit vague. I had to guess what exactly is wanted with the help of the example test output.
	Instead of hard-coding int as data type i would use a typedef or macro for defining the used type.
