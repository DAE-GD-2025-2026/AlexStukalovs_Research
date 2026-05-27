# About
This project is research that serves a single purpose of enabling the author to generate 2D trees using L-systems, which is a problem that has already been solved and has qualitative implementations. This work is targeted at personal development rather than at contributing to the field. It mostly features literature analysis, explaining the concept of L-systems from the ground up and providing open-source C++ projects that demonstrate the application of the theory covered.
This research is one of the exam assignments done by Aleksandrs Stukalovs for the Algorithms 2 course at Howest Digital Arts and Entertainment in the year 2026.
The project and the research are available on the following repository: https://github.com/DAE-GD-2025-2026/AlexStukalovs_Research

# Definition 
**L-Systems**(a.k.a., Lindenmayer Systems) are a mathematical theory of plant development named in honor of biologist Aristid Lindenmayer. L-System is a rewriting system, meaning it defines complex objects by successively replacing parts of a simple initial object using a set of rewriting rules. Note that L-Systems are parallel, as in they apply rules to all the elementary parts of an object simultaneously and not sequentially. *(Prusinkiewicz & Lindenmayer, 1990, p. 1 - 2)*
# Algorithm
The simplest L-system type is **D0L** - **D**eterministic **0**(zero)-context **L**-system. *(Ochoa, 1998)*\
**Deterministic** - each symbol(character) in the system has exactly one rewriting rule *(Daintith & Wright, 2008)*\
**0-context** - the rule is applied without considering any neighboring symbols. *(Lin, 1996)*

Each algorithm is defined by the following data:
* **Axiom** - initial object's form defined by a string. Denoted as $\omega$ (omega). 
* **Productions** - rules applied to objects' parts(characters). Denoted as $p_i, i \in \mathbb{N}$.
	* **Predecessor** - left-hand side of production
	* **Successor** - right-hand side of production

Then the productions are applied to every string that can act as a predecessor.
A constant $n$, $n \in \mathbb{N}$, representing the number of times the productions are applied.
### Example: D0L derivation
$\omega$ : `b`\
$p_1$ : `a -> ab`\
$p_2$ : `b -> a`\
$n$ : `5`\
Then the rewriting process will look as follows:
```
                 b
                 |
                 a
                _|_
               a   b
              _|   \
             a b    a
            _| |    |_
           a b a    a b
          _/  | |_  |_ \   
         a b  a a b a b a
```
**Figure 1.1.** Example derivation of a D0L-system. *(Ochoa, 1998)*.

<video src="https://github.com/user-attachments/assets/e11703fa-5c26-4f3d-b4de-7045dda9c3b7" controls></video>

**Figure 1.2.** User-defined D0L system derivation.

# Graphical representation
To visualize the text-based output of the algorithm, **turtle interpretation of strings** is used. *(Prusinkiewicz & Lindenmayer, 1990, p. 6)*.

> **Turtle graphics** is a simple method for generating computer graphics by entering a series of commands, like "rotate 90 degrees" or "go forward 10 pixels." It mostly became known due to its application in the language "Logo," which was invented by Seymour Papert in the late 1960s to teach basic programming to kids. The name "Turtle graphics" is a reference to one of the early applications of Logo, which was programming of a turtle-like robot. *(Hemmendinger, 2019)*.

According to *Ochoa (1998)*, to interpret strings defined by L-systems, the state of the turtle can be defined by:\
**x, y** -  2D position\
**_a_**     -   Angle(a.k.a, heading)\
And given that **d** - step size and **_b_** - angle increment, the turtle can respond to the following commands(identified by characters):\
`F` - Move forward a step of length **d**. Turtle's location changes to (**x', y'**), where\
       **x'= x + d cos(a)** and **y'= y + d sin(a)**. \
       A line segment between points (**x,y**) and (**x',y'**) is drawn.\
`f`  -  Move forward a step of length **d** without drawing a line.\
`+`  - Turn left by angle **b**. The angle of the turtle changes to **_a + b_**.\
`-`  - Turn right by angle **b**. The angle of the turtle changes to **_a - b_**.

This system is sufficient to visually represent certain L-systems, like the Quadratic Koch island.
### Example: Quadratic Koch island
$\omega$ : `F-F-F-F`\
$p$ : `F -> F+F-F-FF+F+F-F`\
$n$ : `5`\
_a_ : $90^\circ$


https://github.com/user-attachments/assets/6151b92e-5fe0-4a6a-a4b8-3b5fb86e0ce5


**Figure 2.** Quadratic Koch island visualization.

# Bracketed L-systems
The algorithm shown previously is capable of displaying fractals, but it is not capable of creating plant-like structures. The reason is that it always draws a single, uninterrupted line, whereas, for instance, for plant branch generation, it is necessary to be able to interrupt drawing of a certain branch, go back to the stem or trunk, and then proceed with other branches. *(Ochoa, 1998)*.\
It is done by incorporating pushdown automation and using a stack to save the state turtle has to return to after receiving a request to pop from the stack. It is denoted as follows:\
`[` - Push the current state to the stack.\
`]` - Pop the latest state from the stack and make it the current state. (No lines are drawn when the turtle's location changes).\
Except for location and rotation, the state can also contain color and thickness of the lines, for instance. *(Prusinkiewicz & Lindenmayer, 1990, p. 24)*.
### Example: Simple branching structure 
$\omega$ : `F`\
$p$ : `F -> F[+F]F[-F][F]`\
$n$ : `10`\
_a_ : $90^\circ$


https://github.com/user-attachments/assets/7c1b89e8-600c-4766-8dcf-0d6a8f8ae093


**Figure 3.** Simple branching structure visualization.

An attempt to visualize a tree using all the information provided in this research by this moment will look as follows:

$\omega$ : `L`\
$p$ : `L -> F[-L]+L`\
$n$ : `10`\
_a_ : $30^\circ$


https://github.com/user-attachments/assets/07372cd3-9cdc-45ce-af71-f27570c342ce


**Figure 4.** An attempt at tree visualization.

As visible in _Figure 4_, the tree generation does not seem to be believable. There are some aspects that are still to be introduced to make the tree generation more realistic.\
First of all, all the segments are of the same length and width; in reality, it is observable that the branches are incrementally shorter and thinner.\
Secondly, the whole shape is strikingly regular, as it does not have any arbitrariness to it.\
Both of these factors are addressed by adding parameters to this L-system.

# Parametric 0L-systems
**Parameters** - (simplified) variables that modify the effects of certain symbols, e.g., parameter `a` in `+(a)` can determine the absolute value of the angle added to the current state. Note that L-systems are only considered parametric if the arguments used in each instance of a certain symbol are mostly divergent. I.e., defining a segment length that is going to be the same for each instance of `F` as is done, for instance, in Quadratic Koch island, does not make the L-system parametric. On the other hand, if for each instance of `F`, the segment length is half of what it was previously, then the L-system is parametric.\
Parametric systems can be both deterministic(i.e., predictable) or stochastic(i.e., randomized). For instance, in an H-tree _(see Figure 5)_, each subsequent instance of `F` uses a segment length equal to $\sqrt2$ of the previous one, which makes it deterministic. Alternatively, if the line segment length is randomized for each occurrence of the symbol, then it is stochastic.
*(Prusinkiewicz & Lindenmayer, 1990, p. 41)*

$\omega$ : `L`\
$p$ : `L -> [-FL][+FL]`\
$n$ : `10`\
_a_ : $90^\circ$\
_lengthMultiplier_ : $\sqrt 2$ (starting value may vary)


https://github.com/user-attachments/assets/3d58530f-c421-4e3a-8b26-3cb33914310a


**Figure 5.** H-tree visualization.

**NOTE:** One can also define length as _d(g)_ = $2^{-g/2}$, where g - stage idx $\in [0, n)$\. See *(nicebyte, 2018)*. However, in the implementation for this research, the author uses consecutive multiplications by $\sqrt 2$ instead. It is done for the sake of avoiding the necessity to digress to sophisticated parsing.

$\omega$ : `FL`\
$p$ : `L -> [-FL][+FL]`\
$n$ : `8`\
_a_ : $20^\circ$ + Rand($-5^\circ$, $5^\circ$)\
_d_ : 100 / g + Rand($-10^\circ$, $10^\circ$), g - stage idx $\in [0, n)$\
_t_ : 5 / g - thickness

**NOTE:** Here, the necessity for parsing is apparent, yet was still avoided by the author by introducing separate parameters for absolute randomization values, addends, and boolean values to control whether length and thickness have to be divided by the stage index. 



https://github.com/user-attachments/assets/8657fe92-68b7-4cea-a067-23793a465342


**Figure 5.** 2D tree visualization.

# Conclusion
The goal of the research was successfully achieved; the author indeed generated a 2D tree using L-systems. The concept of L-systems once again proved to be a versatile tool, allowing for relatively realistic visual approximation of various organic structures. 

## Limitations and further research possibilities
When it comes to the author's implementation in particular, first and foremost, it would be desired to give users maximal control over the definition of generated L-systems, as well as the ability to save the presets created. Furthermore, modification of the parameters at runtime with instant visual feedback would create significantly more room for experimentation with the tool.\
Furthermore, there will be an inevitable need to invest into parsing. Regarding the features not related to user feedback, the next step would be to add the ability to create multiple rules for the same symbol and provide a probability for the occurrence of each of the rules. This addition would allow for making the systems "more stochastic, " i.e., more random and hence potentially also more realistic.\
Another substantial leap that would potentially allow for more real-world applications for the tool would be to make it work in 3D and provide trees with crowns, consisting of leaves, buds, flowers, etc.

# Source list
* Prusinkiewicz P., Lindenmayer A. (1990). *The Algorithmic Beauty Of Plants*. Springer. https://algorithmicbotany.org/papers/abop/abop.pdf
* Ochoa G. (1998). *An Introduction to Lindenmayer Systems*. The University of Sussex. https://s10.lite.msu.edu/res/msu/botonl/b_online/e28_3/lsys.html
* Daintith J., Wright E. (2008). *A Dictionary of Computing* (6 ed.). Oxford University Press. https://www.oxfordreference.com/display/10.1093/oi/authority.20110803100117366
* Lin T. (1996). *Animation of L-system based 3-D Plant Growing in Java*. University of Maryland. https://userpages.cs.umbc.edu/ebert/693/TLin/top.html
* Hemmendinger D. (2019). *Logo*. Encyclopaedia Britannica, Inc. https://www.britannica.com/print/article/659523
* nicebyte. (2018). Generating trees and other interesting shapes with L-systems. gpfault.net. https://gpfault.net/posts/generating-trees.txt.html
