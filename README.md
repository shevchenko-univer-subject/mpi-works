
# MPI Application Profiling and Visualization Guide

This guide provides instructions on how to profile MPI applications using mpiP and visualize the profiling data using Jumpshot4 and MPE.

## Prerequisites

- [OpenMPI](https://www.open-mpi.org/)
- [MPICH](https://www.mpich.org/)
- [mpiP](https://github.com/LLNL/mpiP): A lightweight MPI profiler.
- [Jumpshot4](https://www.mcs.anl.gov/research/projects/perfvis/download/index.htm#MPE): A tool for visualizing MPI program behavior.
- [MPE](https://cs.calvin.edu/courses/cs/374/MPI/MPE/): Provides tools and libraries for logging MPI events.

Ensure these are installed and properly configured on your system.

## Profiling with mpiP

### Setting Up the Environment

1. Load the MPI module:
   ```sh
   module load mpi/openmpi-x86_64 && export FI_PROVIDER=sockets
   ```
   ```sh
   module load mpi/mpich-x86_64 && export FI_PROVIDER=sockets
   ```

2. Configure mpiP settings:
   ```sh
   export MPIP="-c -p -y -t 1.0 -k 2 -f ./logs"
   ```

   These settings enable concise profiling with point-to-point and collective histograms, a threshold of 1.0%, and a call stack depth of 2. Profiling logs will be saved in the `./logs` directory.

### Compiling Your Application

Compile your MPI application with mpiP:

```sh
mpicc -Wall FILE.c -o FILE -L/usr/local/lib -lmpiP -lmpi -lmpe -lm
```

Replace `FILE.c` with the name of your source file.

### Running Your Application

Run your application with `mpirun`:

```sh
mpirun -np <n> --host fedora:<n> FILE
```

Replace `FILE` with the name of your executable.
Use flag `-x` for setting local env.

### Analyzing Profiling Data

After running your application, mpiP generates profiling data in the specified logs directory. You can analyze these logs for performance insights.

## Visualization with Jumpshot and MPE

### Generating SLOG2 Files

Use MPE to convert mpiP logs to SLOG2 files, which can be visualized using Jumpshot:

1. Follow the [MPE build instructions](https://cs.calvin.edu/courses/cs/374/MPI/MPE/) to install and configure MPE.

2. Use MPE tools to convert mpiP logs to the SLOG2 format.

### Using Jumpshot for Visualization

1. Open the SLOG2 file using Jumpshot4.
2. Use Jumpshot's visualization features to analyze MPI program behavior.

For more information on using Jumpshot, visit the [Jumpshot website](https://www.mcs.anl.gov/research/projects/perfvis/download/index.htm#MPE).

---

## Additional Notes

- Ensure all paths and module names are correct for your specific environment.
- Adjust the number of processes (`-np`) and other mpiP flags according to your profiling requirements.
- For detailed mpiP configuration options, refer to the [mpiP User's Guide](https://github.com/LLNL/mpiP).
