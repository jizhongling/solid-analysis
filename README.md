# solid-analysis

Analysis macros for SoLID detector simulation and reconstruction.
First, you need to compile the solid_dd4hep and solid_recon code.
To run the script, use EIC container version 25.05-stable and run

```bash
./eic-shell -v 25.05-stable
cd solid-analysis
./run_sim_gun <nevents> <index>
```

Step by step to run the SoLID simulation software:

- Run the EIC container version 25.05-stable by the `eic-shell` command shown above.
- Git clone the software [solid_dd4hep](https://github.com/jizhongling/solid_dd4hep) and [solid_recon](https://github.com/jizhongling/solid_recon).
- Compile [solid_dd4hep](https://github.com/jizhongling/solid_dd4hep) and [solid_recon](https://github.com/jizhongling/solid_recon) as shown in their repositories.
- Run the script `run_sim_gun` as shown above. This script will run the solid_dd4hep and solid_recon in sequence.
