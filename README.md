<p align="center">
  <img src="doc/fig/xgnitive-400px_v2.png" alt="XGNITIVE logo" width="300"/>
</p>

# XGNITIVE: A Flexible Cognitive Architecture for Robots

The XGNITIVE repository currently hosts two main lines of research:

### Continuous Goal-Directed Actions (CGDA)
The search for the Grial of generalizing robot actions continues! In the CGDA framework, an action is modelled as the changes it produces on the environment. First, record all the features you can off some user demonstrations. By features, we mean features! The robot joint q2 angle, a human hand Z coordinate, the percentage of a wall painted, the square of the room temperature plus ambient noise... Throw in a demonstration and feature selection algorithm [[1]](#1), let it decide which demonstrations were consistent, and which features are relevant. You now have an action encoded as a CGDA model, which is essentially a multi-dimensional time series. As described in its first conference paper [[2]](#2), recognition can be performed using costs such as those extracted by DTW, and execution can be achieved by evolutionary algorithms in a simulated environment. While we have performed some work in combining sequences of random movements [[3]](#3), we are mostly content with the evolutionary strategies we later developed, such as IET [[4]](#4). Additional references include the original PhD thesis [[5]](#5) and the original repository [[link]](https://github.com/smorante/continuous-goal-directed-actions).

### Robot Imagination System (RIS)
Take a look at most machine learning algorithms, when trying to get something novel, you'll most probably get an average off what you already have. Robot Imagination, as described in its first international conference paper [[6]](#6), aims to generate novel mental models of objects given a previous phase of semantic supervised training. Some improvements can be seen in [[7]](#7). The transition to actions would not be easy, and it eventually lead to the foundations of CGDA research. Additional references include original PhD thesis [[8]](#8).

## Installation
Installation instructions for installing from source can be found [here](https://github.com/roboticslab-uc3m/xgnitive/blob/develop/doc/xgnitive_install_ubuntu_14_04.md).

## APIs

Link to Doxygen generated documentation: http://robots.uc3m.es/dox-xgnitive

<!-- [![xgnitive Homepage](https://img.shields.io/badge/xgnitive--orange.svg)](http://robots.uc3m.es/dox-xgnitive) [![Latest Release](https://img.shields.io/github/tag/roboticslab-uc3m/xgnitive.svg?label=Latest Release)](https://github.com/roboticslab-uc3m/xgnitive/tags) -->

## References
<a id="1">[1]</a> S. Morante, J.G. Victores, C. Balaguer. Automatic Demonstration and Feature Selection for Robot Learning. IEEE International Conference on Humanoid Robots (Humanoids 2015), pages: 428 - 433, IEEE. 2015-11-03, Seoul, South Korea. 2015.

<a id="2">[2]</a> S. Morante, J.G. Victores, A. Jardon, C. Balaguer. Action Effect Generalization, Recognition and Execution through Continuous Goal-Directed Actions. IEEE International Conference on Robotics and Automation (ICRA 2014), pages: 1822 - 1827, IEEE. 2014-06-03, Hong Kong, China. 2014.

<a id="3">[3]</a> S. Morante, J.G. Victores, A. Jardon, C. Balaguer. On Using Guided Motor Primitives to Execute Continuous Goal-Directed Actions. IEEE international symposium on robot and human interactive communication (RO-MAN 2014). , ISBN: 978-1-4799-6763-6, pages: 613 - 618, IEEE. 2014-08-28, Edinburgh, Scotland. 2014.

<a id="4">[4]</a> S. Morante, J.G. Victores, A. Jardon, C. Balaguer. Humanoid Robot Imitation through Continuous Goal-Directed Actions : An Evolutionary Approach. Advanced Robotics, vol: 29, number: 5, pages: 303 - 314. 2015.

<a id="5">[5]</a> S. Morante. PhD Thesis. Continuous Goal-Directed Actions: Advanced in Robot Learning. Supervisor: C. Balaguer, J.G. Victores. Qualification: Cum Laude. Universidad Carlos III de Madrid. 2016.

<a id="6">[6]</a>  J.G. Victores, S. Morante, A. Jardon, C. Balaguer. Towards Robot Imagination Through Object Feature Inference. IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS 2013), ISBN: 2153-0858, pages: 5694 - 5699, IEEE. 2013-11-05, Tokyo, Japan. 2013.

<a id="7">[7]</a>  J.G. Victores, S. Morante, A. Jardon, C. Balaguer. On Using Humanoid Robot Imagination to Perform the Shortened Token Test. IEEE RAS International Conference on Humanoid Robots (Humanoids 2014), pages: 172 - 172, IEEE. 2014-11-18, Madrid, Spain. 2014.

<a id="8">[8]</a>  J.G. Victores. Robot Imagination System. PhD Thesis. Supervisor: C. Balaguer, A. Jardon. Qualification: Cum Laude. Universidad Carlos III de Madrid. 2014.
