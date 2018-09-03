from rllab.algos.ddpg import DDPG
from envs.teo_paint_discrete_yarp_env import TeoPaintDiscreteYarpEnv
from rllab.envs.normalized_env import normalize
from rllab.policies.categorical_mlp_policy import CategoricalMLPPolicy
from rllab.misc.instrument import run_experiment_lite

from rllab.exploration_strategies.ou_strategy import OUStrategy #We use OU strategy to add noise for exploration
from rllab.q_functions.continuous_mlp_q_function import ContinuousMLPQFunction #Only q function implemented in rllab

def run_task(*_):
    env = normalize(TeoPaintDiscreteYarpEnv())
    policy = CategoricalMLPPolicy(
        env_spec=env.spec,
    )

    es = OUStrategy(env_spec=env.spec)

    qf = ContinuousMLPQFunction(env_spec=env.spec)

    algo = DDPG(
        env=env,
        policy=policy,
        es=es,
        qf=qf,
        batch_size=32,
        max_path_length=150,
        epoch_length=1000,
        min_pool_size=10000,
        n_epochs=1000,
        discount=0.99,
        scale_reward=0.01,
        qf_learning_rate=1e-3,
        policy_learning_rate=1e-4,
        # Uncomment both lines (this and the plot parameter below) to enable plotting
        # plot=True,
    )
    algo.train()

run_experiment_lite(
    run_task,
    # Number of parallel workers for sampling
    n_parallel=1,
    # Only keep the snapshot parameters for the last iteration
    snapshot_mode="last",
    # Specifies the seed for the experiment. If this is not provided, a random seed
    # will be used
    # seed=1,
    # plot=True,
)
