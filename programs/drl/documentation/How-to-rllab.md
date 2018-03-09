# How-to-rllab 

So this document is intended to be as a "how to use" rllab documentation. Its still under development so may be incomplete parts, all the support is also welcome.

So the first question to answer is **"What is rllab?"**: from the [official documentation](http://rllab.readthedocs.io/en/latest/) is a *"framework for developing and evaluating reinforcement learning algorithms"*. For us, we use it a a python framework to work with DEEP REINFORCEMENT LEARNING systems, by developing our own algorithms, environments and so on, or just using the ones already implemented in the framework.

Ok, so thats clear, the next question is then **How to use rllab**. To use rllab the basic code for every DRL system is the following: 

```
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

**Lets dissect this code:**

```
from rllab.algos.trpo import TRPO 
```
This loads the **RL algorithm** we want to use, in this case a Trust Region Policy (TRPO) algorithm will be used.

```
from rllab.baselines.linear_feature_baseline import LinearFeatureBaseline 
```
This loads the **baseline** we want to use, so what is a baseline. In few words, a baseline is something you use to speed up the convergence. The idea behind this is that in RL good solutions obtain a positive reward +R, while bad solutions obtain negative rewards -R. However in the practice most of the solution are positive, so the convergence is quite slow. To avoid this a baseline is added to the algorithm, solution better than the baseline are assigned a positive reward +R, while the other are assigned with a negative reward, speeding up the convergence to the best ones.

```
from rllab.envs.box2d.cartpole_env import CartpoleEnv 
```
Here the environment we want to use as the training environment is loaded. **This environemnt contains the information about the agent, the environment itsel and the reward function.**

```
from rllab.envs.normalized_env import normalize 
```

This loads the **normalize** function, to make sure that the actions for the environment lies within the range [-1, 1] (only works for environments with continuous actions).

```
from rllab.policies.gaussian_mlp_policy import GaussianMLPPolicy 
```

Here the **policy estimator** is loaded, in this case a GaussianMLPPolicy is used.

```
policy = GaussianMLPPolicy( #Definimos la policy con una red neuronal.
	env_spec=env.spec,
	hidden_sizes=(32, 32)
)
```
Defines the policy used by the algorithm and initialize its values. Here a GaussianMLPPolicy with 2 hidden layers, each with 32 hidden units is used.


