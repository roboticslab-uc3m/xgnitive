from rllab.algos.trpo import TRPO
from rllab.baselines.linear_feature_baseline import LinearFeatureBaseline
#from rllab.envs.grid_world_env import GridWorldEnv
from envs.teo_paint_discrete_yarp_env import TeoPaintDiscreteYarpEnv
from rllab.envs.normalized_env import normalize
from rllab.policies.categorical_mlp_policy import CategoricalMLPPolicy
from rllab.misc.instrument import run_experiment_lite

def run_task(*_):
    env = normalize(TeoPaintDiscreteYarpEnv())
    policy = CategoricalMLPPolicy(
        env_spec=env.spec,
    )
    baseline = LinearFeatureBaseline(env_spec=env.spec)
    algo = TRPO(
        env=env,
        policy=policy,
        baseline=baseline,
        batch_size=1000, #The more, the more past samples it takes.
        max_path_length=400, #Lenght of the learned path.
        discount=0.99, #discount for discounter
        step_size=0.01, #Step size of the gradient
        #n_itr=800,
        # Uncomment both lines (this and the plot parameter below) to enable plotting
        #plot=True,
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
