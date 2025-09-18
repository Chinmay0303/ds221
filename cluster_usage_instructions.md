# Teaching Cluster Instructions

## Login Instructions
1. **SSH Address:** `10.24.1.10`
2. **Username:** Use your IISc email ID **without** `@iisc.ac.in`
3. **Password:** `defaultpassword`

**Example Login:** 
(If the iisc email id of student is: parveshbarak@iisc.ac.in)
```bash
ssh parveshbarak@10.24.1.10
parveshbarak@10.24.1.10’s password: defaultpassword
```

## Note:
- <b>You must change the password  </b> by using the command `passwd` on your first login, to your preference but make sure the passwords are strong with a minimum of 6 characters, with uppercase/lowecase letters, numbers and special character combinations. You may risk exposing your code to others if the password is left unchanged, which could result in penalties.
- Also a dedicated folder `scratch/username` has been created. Use this space to store your data, as the home directory has limited storage capacity.

### Refer to [cluster_schedule.md](cluster_schedule.md) to check your assigned slot and node

## Usage Guidelines
- Store all files in `/scratch/username`
- Do not use `/home/username` for storing files.
- Run your code only on your allotted compute node.
- The master node is for login only — do not run jobs on it.
- Use the cluster only during your assigned time slot.
- Time slots are randomly chosen and rotated. Please do not bring requests to change.
- To run your jobs on compute nodes, you would first need to create a job file (see [example_script.md](example_script.sh) for a toy example).
- You can schedule your job for running by using `sbatch your_job_script.sh` command.
- To see the status of your jobs, you may use `squeue --user username` command.
- For other commands specific to SLURM job scheduler used by the teaching cluster, please check [slurm documentation](https://slurm.schedmd.com/overview.html).

## Warnings & Penalties
- **A 20% marks penalty will be imposed if:**
    - You use another student’s time slot or compute node.
    - You run jobs or code on the master node.
    - You store files in `/home/username` instead of `/scratch/username`.
    - Cluster logs will be regularly monitored to detect violations.
