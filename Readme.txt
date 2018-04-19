How to run the code:
- source run_AllPlots.sh
  it calls run_plotter_parallel.sh where are lists all the samples and plotter_vbfzll.C where are definited histos, selection,
  cuts, tmva and so on.
- in histoFileDir:
  source hadd_histos.sh --> it will merge some files (like DY and SingleTop)
- in plotCode:
  source plot_nopas.sh ---> it will create validation plots in plotsDirectory
