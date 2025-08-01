# solid-analysis

Analysis macros for SoLID detector simulation and reconstruction.
First, you need to compile the solid_dd4hep and solid_recon code.
To run the script, use EIC container version 25.05-stable and run

```bash
./eic-shell -v 25.05-stable
export PATH=$EIC_SHELL_PREFIX/bin:$PATH
export LD_LIBRARY_PATH=$EIC_SHELL_PREFIX/lib:/opt/local/lib:/opt/local/lib64
cd solid-analysis
./run_sim_gun <nevents> <index>
```

Step by step to run the SoLID simulation software:
- Install the `eic-shell` by `curl --location https://get.epic-eic.org | bash`
- Update the `eic-shell` by `./eic-shell -u -v 25.05-stable`
- Run the EIC container version 25.05-stable by the `./eic-shell -v 25.05-stable`
- Git clone the software [solid_dd4hep](https://github.com/jizhongling/solid_dd4hep) and [solid_recon](https://github.com/jizhongling/solid_recon).
- Compile [solid_dd4hep](https://github.com/jizhongling/solid_dd4hep) and [solid_recon](https://github.com/jizhongling/solid_recon) as shown in their repositories.
- Run the script `run_sim_gun` as shown above. This script will run the solid_dd4hep and solid_recon in sequence.
