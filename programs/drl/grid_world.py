from rllab.algos.trpo import TRPO
from rllab.baselines.linear_feature_baseline import LinearFeatureBaseline
#from rllab.envs.grid_world_env import GridWorldEnv
from envs.grid_world_env import GridWorldEnv
from rllab.envs.normalized_env import normalize
from rllab.policies.categorical_mlp_policy import CategoricalMLPPolicy

env = normalize(GridWorldEnv())
policy = CategoricalMLPPolicy(
    env_spec=env.spec,
)
baseline = LinearFeatureBaseline(env_spec=env.spec)
algo = TRPO(
    env=env,
    policy=policy,
    baseline=baseline,
    # Uncomment both lines (this and the plot parameter below) to enable plotting
    #plot=True,
)
algo.train()
