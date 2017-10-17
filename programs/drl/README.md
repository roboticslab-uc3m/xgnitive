# DRL Execution

The goal of this repository is to develop DRL inside the CGDA framework.

## Dependencies

This repository uses **rllab** as the framework for the developing DRL algorithms. The instructions to install rllab can be found [here](http://rllab.readthedocs.io/en/latest/user/installation.html).

**Add YARP inside rllab environment**
```bash
cp /usr/local/lib/python3.5/site-packages/yarp.py  ~/anaconda2/envs/rllab3/lib/python3.5/site-packages/
cp /usr/local/lib/python3.5/site-packages/_yarp.so  ~/anaconda2/envs/rllab3/lib/python3.5/site-packages/
```

**To start rllab, run the following commands in a terminal:**

```
source activate rllab3 #Initialize rllab
export PYTHONPATH=/home/raul/repos/rllab:$PYTHONPATH #Export rllab to pythonpath

```


## Tutorials: 2D grid world example

First train the policy using DRL

```
python grid_2D_world.py
```

The policy params are then saved in the following paths *data/local/experiment_{timestamp}* of your local rllab repository. To simulate an execution of the task using the policy trained, you can use the following commands:

```
python sim_policy.py home/repos/rllab/data/local/experiment_{timestamp}/params.pkl
```

## Tutorials: 3D grid world example

First train the policy using DRL

```
python grid_3D_world.py
```

The policy params are then saved in the following paths *data/local/experiment_{timestamp}* of your local rllab repository. To simulate an execution of the task using the policy trained, you can use the following commands:

```
python sim_policy.py home/repos/rllab/data/local/experiment_{timestamp}/params.pkl
```
