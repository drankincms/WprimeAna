# for model building:
def get_model():
    # Read in and build the model automatically from the histograms in the root file. 
    # This model will contain all shape uncertainties given according to the templates
    # which also includes rate changes according to the alternate shapes.
    # For more info about this model and naming conventuion, see documentation
    # of build_model_from_rootfile.
    model = build_model_from_rootfile('Wprime_Histos_NAME-AL-AR.root',include_mc_uncertainties=True)

    # If the prediction histogram is zero, but data is non-zero, teh negative log-likelihood
    # is infinity which causes problems for some methods. Therefore, we set all histogram
    # bin entries to a small, but positive value:
    model.fill_histogram_zerobins()

    # define what the signal processes are. All other processes are assumed to make up the 
    # 'background-only' model.
    model.set_signal_processes('wp*')
    
    # Add some lognormal rate uncertainties. The first parameter is the name of the
    # uncertainty (which will also be the name of the nuisance parameter), the second
    # is the 'effect' as a fraction, the third one is the process name. The fourth parameter
    # is optional and denotes the channl. The default '*' means that the uncertainty applies
    # to all channels in the same way.
    # Note that you can use the same name for a systematic here as for a shape
    # systematic. In this case, the same parameter will be used; shape and rate changes 
    # will be 100% correlated.
    
    #for p in model.processes:
    #    if p == 'tops': 
    #        model.add_lognormal_uncertainty('scaled_rate',  math.log(1.30), p)
    #    if p == 'topsntb': 
    #        model.add_lognormal_uncertainty('scaled_rate',  math.log(1.30), p)
    #    if p.startswith('wp'):
    #        model.add_lognormal_uncertainty('tau_rate',  math.log(1.02), p)                        

    model.add_lognormal_uncertainty('wjets_rate',  math.log(1.10), 'wjets')
    model.add_lognormal_uncertainty('ttbar_rate',  math.log(1.08), 'ttbar')
    #model.add_lognormal_uncertainty('qcd_rate',    math.log(1.30), 'qcd')
    ELCOMmodel.add_lognormal_uncertainty('elepeff', math.log(1.02), '*','elec_soft_ex1btags')
    ELCOMmodel.add_lognormal_uncertainty('ehlteff', math.log(1.02), '*','elec_soft_ex1btags')
    MUCOMmodel.add_lognormal_uncertainty('mulepeff', math.log(1.02), '*','mu_soft_ex1btags')
    MUCOMmodel.add_lognormal_uncertainty('muhlteff', math.log(1.02), '*','mu_soft_ex1btags')
    ELCOMmodel.add_lognormal_uncertainty('elepeff', math.log(1.02), '*','elec_soft_ex2btags')
    ELCOMmodel.add_lognormal_uncertainty('ehlteff', math.log(1.02), '*','elec_soft_ex2btags')
    MUCOMmodel.add_lognormal_uncertainty('mulepeff', math.log(1.02), '*','mu_soft_ex2btags')
    MUCOMmodel.add_lognormal_uncertainty('muhlteff', math.log(1.02), '*','mu_soft_ex2btags')
    ELCOMmodel.add_lognormal_uncertainty('elepeff', math.log(1.02), '*','elec_hard_ex1btags')
    ELCOMmodel.add_lognormal_uncertainty('ehlteff', math.log(1.02), '*','elec_hard_ex1btags')
    MUCOMmodel.add_lognormal_uncertainty('mulepeff', math.log(1.02), '*','mu_hard_ex1btags')
    MUCOMmodel.add_lognormal_uncertainty('muhlteff', math.log(1.02), '*','mu_hard_ex1btags')
    ELCOMmodel.add_lognormal_uncertainty('elepeff', math.log(1.02), '*','elec_hard_ex2btags')
    ELCOMmodel.add_lognormal_uncertainty('ehlteff', math.log(1.02), '*','elec_hard_ex2btags')
    MUCOMmodel.add_lognormal_uncertainty('mulepeff', math.log(1.02), '*','mu_hard_ex2btags')
    MUCOMmodel.add_lognormal_uncertainty('muhlteff', math.log(1.02), '*','mu_hard_ex2btags')

    # the qcd model is from data, so do not apply a lumi uncertainty on that:
    for p in model.processes:
         model.add_lognormal_uncertainty('lumi',  math.log(1.025), p)
         #model.add_lognormal_uncertainty('trigger',  math.log(1.03), p)
         #model.add_lognormal_uncertainty('lepton_id',  math.log(1.03), p)
         print p

    # Specifying all uncertainties manually can be error-prone. You can also execute
    # a automatically generated file using python's execfile here
    # which contains these statements, or read in a text file, etc. Remember: this is a
    # python script, so use this power!
    #model.scale_predictions(1.04976)

    #for p in model.get_parameters(model.signal_processes):
    #    model.distribution.set_distribution(p,'gamma', mean = '0.', width = '1.0', range = [-float("inf"), float("inf")])
    model.distribution.set_distribution('toppt','gauss', mean = 0., width = 1.0, range = [float("0.0"), float("inf")])

    return model

model = get_model()


# first, it is a good idea to generate a summary report to make sure everything has worked
# as expected. The summary will generate quite some information which should it make easy to spot
# errors like typos in the name of uncertainties, missing shape uncertaintie, etc.
model_summary(model)

# 2. apply the methods

# 2.a. Bayesian limits
# Calculate expected and observed Bayesian limits. For faster run time of this example,
# only make a few mass points. (Omitting the 'signal_procsses' parameter completely would
# process all signals defined as signal processes before; see Section "Common Parameters"
# on the theta auto intro doxygen page for details)
#plot_exp, plot_obs = bayesian_limits(model, signal_processes = [['wp2000'],['wp2200'],['wp2500'],['wp2800'],['wp3000']])
plot_exp, plot_obs = bayesian_limits(model,'all')

# plot_exp and plot_obs are instances of plotutil.plotdata. they contain x/y values and
# bands. You can do many things with these objects such as inspect the x/y/ban
# data, pass then to plotutil.plot routine to make pdf plots, ...
# Here, we will just create text files of the plot data. This is useful if you want
# to apply your own plotting routines or present the result in a text Table.
plot_exp.write_txt('bayesian_limits_expected_AL-AR.txt')
plot_obs.write_txt('bayesian_limits_observed_AL-AR.txt')

