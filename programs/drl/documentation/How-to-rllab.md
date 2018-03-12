# How-to-rllab 

So this document is intended to be as a "how to use" rllab documentation. Its still under development so may be incomplete parts, all the support is also welcome.

## What is rllab

So the first question to answer is **"What is rllab?"**: from the [official documentation](http://rllab.readthedocs.io/en/latest/) is a *"framework for developing and evaluating reinforcement learning algorithms"*. For us, we use it a a python framework to work with DEEP REINFORCEMENT LEARNING systems, by developing our own algorithms, environments and so on, or just using the ones already implemented in the framework.

## How to use rllab
Ok, so thats clear, the next question is then **How to use rllab**. To use rllab the basic code for every DRL system is the following: 
_______________________________________________________________________
```python
from rllab.algos.trpo import TRPO 
from rllab.baselines.linear_feature_baseline import LinearFeatureBaseline 
from rllab.envs.box2d.cartpole_env import CartpoleEnv 
from rllab.envs.normalized_env import normalize

env = normalize(CartpoleEnv())

policy = GaussianMLPPolicy(
	env_spec=env.spec,
	hidden_sizes=(32, 32)
)

baseline = LinearFeatureBaseline(env_spec=env.spec)

algo = TRPO(
	env=env,
	policy=policy,
	baseline=baseline,
	batch_size=4000,
	whole_paths=True,
	max_path_length=100,
	n_itr=40,
	discount=0.99,
	step_size=0.01,
)

algo.train()

```
___

Lets dissect this code:

```python
from rllab.algos.trpo import TRPO 
```

This loads the **RL algorithm** we want to use, in this case a Trust Region Policy (TRPO) algorithm will be used.

```python
from rllab.baselines.linear_feature_baseline import LinearFeatureBaseline 
```
This line loads the **baseline** we want to use, so what is a baseline. In few words, a baseline is something you use to speed up the convergence. The idea behind this is that in RL good solutions obtain a positive reward +R, while bad solutions obtain negative rewards -R. However in the practice most of the solution are positive, so the convergence is quite slow. To avoid this a baseline is added to the algorithm, solution better than the baseline are assigned a positive reward +R, while the other are assigned with a negative reward, speeding up the convergence to the best ones.

```python
from rllab.envs.box2d.cartpole_env import CartpoleEnv 
```
Here the environment we want to use as the training environment is loaded. **This environemnt contains the information about the agent, the environment itsel and the reward function.**

```python
from rllab.envs.normalized_env import normalize 
```

This loads the **normalize** function, to make sure that the actions for the environment lies within the range [-1, 1] (only works for environments with continuous actions).

```python
from rllab.policies.gaussian_mlp_policy import GaussianMLPPolicy 
```

Here the **policy estimator** is loaded, in this case a GaussianMLPPolicy is used.

```python
policy = GaussianMLPPolicy( #Definimos la policy con una red neuronal.
	env_spec=env.spec,
	hidden_sizes=(32, 32)
)
```
Defines the policy used by the algorithm and initialize its values. Here a **GaussianMLPPolicy** with 2 hidden layers, each with 32 hidden units is used.

```python
algo = TRPO(
	env=env,
	policy=policy,
	baseline=baseline,
	batch_size=4000,
	whole_paths=True,
	max_path_length=100,
	n_itr=40,
	discount=0.99,
	step_size=0.01,
)
```

This part of the code corresponds with the initialisation of the algorithm. This will be further explained in section **How to define the algorithm.**

What we can conclude is that in order to implement our own DRL program we need to define three major parts:

- **The RL algorithm.**
- **The environment.**
- **The policy.**

## How to define the algorithm

Here we have two options, [implement our own algorithm](http://rllab.readthedocs.io/en/latest/user/implement_algo_advanced.html), or just using an already implemented one from the rllab library. The first part, right now, is out of the scope of this documentation. In this section we are going to review the main algorithms presented in rllab.

In rllab the algorithms are divided in two groups:

- **Batch algorithms:** Its a subgroup of algorithms inside the dynamic programming algorithms. These are:
	- **REINFORCE**: Algorithm from 1992. *Williams, R.J. Mach Learn (1992) 8: 229. https://doi.org/10.1007/BF00992696*
	- **Truncated Natural Policy Gradient (TNPG)**: *Kakade, S. M. (2002). A natural policy gradient. In Advances in neural information processing systems (pp. 1531-1538)*.
	- **Trust Region Policy Optimization (TRPO)**: This algorithm is effective for optimizing large nonlinear policies such as neural networks. The idea of TRPO is that you have some function that you want to approximate. Then you a have a local approximation which is quite accuarate, and the you have the farther you go out of that approximation the more inaccuarate it gets. The limitations are that is Hard to use in architectures with multiple outputs, performs poorly in task requiring deep CNN and RNN (like atari) and having a conjugate gradient makes all more complicated. *Schulman, J., Levine, S., Moritz, P., Jordan, M. I., & Abbeel, P. (2015). Trust Region Policy Optimization. https://doi.org/10.1063/1.4927398*
	- **Relative Entropy Policy Search(REPS):** This method limits the information loss between iterations to achieve a soft learning curve. *Peters, J., Mülling, K., & Altun, Y. (2010, July). Relative Entropy Policy Search. In AAAI (pp. 1607-1612).*
	- **Cross Entropy Method (CEM):** RL method from 1999. Rubinstein, R. Y. (1999). The cross-entropy method for combinatorial and continuous optimization. Methodology and Computing in Applied Probability, 1(2), 127–190.
	- **Covariance Matrix Adaptation Evolution Strategy (CMA-ES)** This is a RL based on evolutionary algorithms. *Nikolaus Hansen, Sibylle D. Müller, and Petros Koumoutsakos. 2003. Reducing the time complexity of the derandomized evolution strategy with covariance matrix adaptation (CMA-ES). Evol. Comput. 11, 1 (March 2003), 1-18. DOI=http://dx.doi.org/10.1162/106365603321828970* 

 - **Online Algorithms**
 	- **Deep determisitc Policy Gradient (DDPG):** This algorithms was born to be able to use Q-learning with a  continous action space. *Bengio, Y. (2016). Continuous control with deep reinforcement learning. Foundations and Trends® in Machine Learning, 2(1), 1–127. https://doi.org/10.1561/2200000006*
	
## How to define the environment

In rllab to define an environment we have to define at least the following functions:

- **observation_space(self)** : This function is where we define the observation space of our problem. This function returns this observation space. In rllab there are three defined types, [Box](https://github.com/rll/rllab/blob/master/rllab/spaces/box.py), [Discrete](https://github.com/rll/rllab/blob/master/rllab/spaces/discrete.py), and [product](https://github.com/rll/rllab/blob/master/rllab/spaces/product.py). This way, supposing a grid world with *n_row.n_col* cells, the observation space is defined as:


```python
    def observation_space(self):
	return Discrete(self.n_row * self.n_col)
```



- **action_space(self):** This function is where we define the action space of our problem. This function returns this observation space. Following with the grid world idea, an example of this could be an action space with 4 discrete actions, up, right, left and dow.

```python
    def action_space(self):
	return Discrete(4)
```

- **step(self,action):** This function takes as input the state and the action we want to perform, and returns the next state, the reward, and if it has finished. In the grid world this could be something like this:

```python
	def step(self, action):
		next_state=self.get_next_state(self, action) #This function returns the next state for a given action "up,right,left,down"

		#H="death", F="Free", S="Start", G="Goal"
		if next_state_type == 'H':
		    done = True
		    reward = 0
		elif next_state_type in ['F', 'S']:
		    done = False
		    reward = 0
		elif next_state_type == 'G':
		    done = True
		    reward = 1
		else:
		    raise NotImplementedError
		self.state = next_state
		return Step(observation=self.state, reward=reward, done=done)

```

 - **reset(self)** Reset the system to a initial state and returns the observation of that state. In the grid example, the observation is the state where the robot is. This way, having defined an initial state (self.start_state), this could be something like this: 
 
 ```python
     self.state = self.start_state
	return self.state
 ```
 
 ## How to define the policy
 
[Example](https://github.com/roboticslab-uc3m/xgnitive/blob/485c982b16403c2ac78f2816fae4b560e71d5b46/programs/drl/envs/grid_world_2D_env.py)

# Sources:
- *Duan, Y., Chen, X., Schulman, J., & Abbeel, P. (2016). Benchmarking Deep Reinforcement Learning for Continuous Control. arXiv, 48, 14. Retrieved from http://arxiv.org/abs/1604.06778*

- http://rllab.readthedocs.io/en/latest/
- *Deep RL Bootcamp videos from:* https://www.youtube.com/channel/UCTgM-VlXKuylPrZ_YGAJHOw






