# for model building:
def get_model():
    # Read in and build the model automatically from the histograms in the root file. 
    # This model will contain all shape uncertainties given according to the templates
    # which also includes rate changes according to the alternate shapes.
    # For more info about this model and naming conventuion, see documentation
    # of build_model_from_rootfile.
    model = build_model_from_rootfile('../RootFiles_For2DLimits/comb_mWprime_Limits_WprimeRight_HardSoft_Histos-ex1or2btags.root',include_mc_uncertainties=True)

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
    model.add_lognormal_uncertainty('elepeff', math.log(1.02), '*','elec_soft_ex1btags')
    model.add_lognormal_uncertainty('ehlteff', math.log(1.02), '*','elec_soft_ex1btags')
    model.add_lognormal_uncertainty('mulepeff', math.log(1.02), '*','mu_soft_ex1btags')
    model.add_lognormal_uncertainty('muhlteff', math.log(1.02), '*','mu_soft_ex1btags')
    model.add_lognormal_uncertainty('elepeff', math.log(1.02), '*','elec_soft_ex2btags')
    model.add_lognormal_uncertainty('ehlteff', math.log(1.02), '*','elec_soft_ex2btags')
    model.add_lognormal_uncertainty('mulepeff', math.log(1.02), '*','mu_soft_ex2btags')
    model.add_lognormal_uncertainty('muhlteff', math.log(1.02), '*','mu_soft_ex2btags')

    model.add_lognormal_uncertainty('elepeff', math.log(1.02), '*','elec_hard_ex1btags')
    model.add_lognormal_uncertainty('ehlteff', math.log(1.02), '*','elec_hard_ex1btags')
    model.add_lognormal_uncertainty('mulepeff', math.log(1.02), '*','mu_hard_ex1btags')
    model.add_lognormal_uncertainty('muhlteff', math.log(1.02), '*','mu_hard_ex1btags')
    model.add_lognormal_uncertainty('elepeff', math.log(1.02), '*','elec_hard_ex2btags')
    model.add_lognormal_uncertainty('ehlteff', math.log(1.02), '*','elec_hard_ex2btags')
    model.add_lognormal_uncertainty('mulepeff', math.log(1.02), '*','mu_hard_ex2btags')
    model.add_lognormal_uncertainty('muhlteff', math.log(1.02), '*','mu_hard_ex2btags')

    # the qcd model is from data, so do not apply a lumi uncertainty on that:
    for p in model.processes:
         model.add_lognormal_uncertainty('lumi',  math.log(1.026), p)
         #model.add_lognormal_uncertainty('trigger',  math.log(1.03), p)
         #model.add_lognormal_uncertainty('lepton_id',  math.log(1.03), p)
         #print p

    # Specifying all uncertainties manually can be error-prone. You can also execute
    # a automatically generated file using python's execfile here
    # which contains these statements, or read in a text file, etc. Remember: this is a
    # python script, so use this power!
    #model.scale_predictions(1.04976)
    #for p in model.get_observables():
         #print p
         #if (str(p).startswith("elec")):
             #model.scale_predictions(0.95, '*', p)

    #for p in model.get_parameters(model.signal_processes):
    #    model.distribution.set_distribution(p,'gamma', mean = '0.', width = '1.0', range = [-float("inf"), float("inf")])
    #model.distribution.set_distribution('toppt','gauss', mean = 0., width = 1.0, range = [float("0.0"), float("inf")])

#    model.rebin('elec_soft_ex1btags',2)
#    model.rebin('elec_soft_ex1btags',2)
#    model.rebin('mu_soft_ex1btags',2)
#    model.rebin('mu_soft_ex1btags',2)
#    model.rebin('elec_soft_ex2btags',2)
#    model.rebin('elec_soft_ex2btags',2)
#    model.rebin('mu_soft_ex2btags',2)
#    model.rebin('mu_soft_ex2btags',2)
#
#    model.rebin('elec_hard_ex1btags',2)
#    model.rebin('elec_hard_ex1btags',2)
#    model.rebin('mu_hard_ex1btags',2)
#    model.rebin('mu_hard_ex1btags',2)
#    model.rebin('elec_hard_ex2btags',2)
#    model.rebin('elec_hard_ex2btags',2)
#    model.rebin('mu_hard_ex2btags',2)
#    model.rebin('mu_hard_ex2btags',2)

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
plot_exp.write_txt('comb_bayesian_limits_expected_ex1or2btags_hardsoft.txt')
plot_obs.write_txt('comb_bayesian_limits_observed_ex1or2btags_hardsoft.txt')

# 2.b. CLs limits
# calculate cls limit plots. The interface is very similar to bayesian_limits. However, there are a few
# more options such as the definition of the test statistic which is usually a likelihood ratio but can differ in
# which parameters are minimized and which constraints / ranges are applied during minimization.
# Here, we stay with the default which fixes beta_signal=0
# for the background only hypothesis and lets it float freely for the signal+background hypothesis.
# See cls_limits documentation for more options.
#plot_exp, plot_obs = cls_limits(model, signal_processes = [['wpglr800'], ['wpglr900'], ['wpglr1000'], ['wpglr1100'], ['wpglr1200'], ['wpglr1300'], ['wpglr1400'], ['wpglr1500'], ['wpglr1600'] ])
#cls_limits(model, ts='lhclike',run_theta=False)


# as for the bayesian limits: write the result to a text file
#plot_exp.write_txt('cls_limits_expected.txt')
#plot_obs.write_txt('cls_limits_observed.txt')

#calculate expected Z-value for discovery potential
signal_process_groups = {'wp2000': ['wp2000'], 'wp2500': ['wp2500']}
#import json
#f = open('electron_ex1or2btags_discovery.json', 'w')
#disc = discovery(model,use_data = False,input_expected='toys:0.114590',spid='wp2000',Z_error_max=0.1,ts_method=derll)
#print disc
#json.dump(disc, f)
#f.write('\n')
#disc = discovery(model,use_data = False,input_expected='toys:0.033343',spid='wp2500',Z_error_max=0.1,ts_method=derll)
#print disc
#json.dump(disc, f)
#f.write('\n')
#disc = discovery(model,use_data = False,input_expected='toys:0.010756',spid='wp3000',Z_error_max=0.1,ts_method=derll)
#print disc
#json.dump(disc, f)
#f.close()

#print model.get_parameters(['wp2000','wp2500'])

signal_process_groups = {'': []}

options = Options()
options.set('minimizer', 'strategy', 'robust')
options.set('minimizer', 'minuit_tolerance_factor', '100')
options.set('global', 'debug', 'True')

parVals = mle(model, input = 'data', n=1, signal_process_groups = signal_process_groups, options = options)
#parVals = mle(model, input = 'toys:1.0', n=10000, signal_process_groups = signal_process_groups, options = options)
print parVals
import json
f = open('comb_ex1or2btags_hardsoft_postfit.json', 'w')
json.dump(parVals[''], f)
f.close()
parameter_values = {}
for p in model.get_parameters([]):
    parameter_values[p] = parVals[''][p][0][0]
histos = evaluate_prediction(model, parameter_values, include_signal = False)
write_histograms_to_rootfile(histos, '../RootFiles_For2DLimits/histos_comb_ex1or2btags_hardsoft-mle.root')

p = open('comb_ex1or2btags_hardsoft_unc.json', 'w')
p.write('{')
firstloop = True
posthist = bayesian_posterior_model_prediction(model, input = 'data', n=1, signal_process_groups = signal_process_groups, options = options)
for cname in ['elec','mu']:
    for bname in ['ex1btags','ex2btags']:
        for ename in ['hard','soft']:
            if (not cname+'_'+ename+'_'+bname in posthist['']):
                continue
            if (not firstloop):
                p.write(', ')
            else:
                firstloop = False
            p.write('"'+cname+'_'+ename+'_'+bname+'": [')
            phist = posthist[''][cname+'_'+ename+'_'+bname][0]
            #print cname+'_'+ename+'_'+bname
            #print phist.get_values()
            p.write(str(phist.get_values()).replace('''array('d', ''','').replace(')',''))
            p.write(', ')
            #print phist.get_uncertainties()
            p.write(str(phist.get_uncertainties()).replace('''array('d', ''','').replace(')',''))
            p.write(']')
p.wirte('}')
p.close()


#parScan = nll_scan(model, input = 'data', n = 1, range = [0.0, 3.0], parameter='toppt', signal_process_groups = signal_process_groups, options = options)
#print parScan
#for p in parScan['']:
#    p.write_txt('comb_ex1or2btags_hardsoft_topptscan.txt')
#
#parScan = nll_scan(model, input = 'data', n = 1, range = [-3.0, 3.0], parameter='murmuf', signal_process_groups = signal_process_groups, options = options)
#print parScan
#for p in parScan['']:
#    p.write_txt('comb_ex1or2btags_hardsoft_murmufscan.txt')
#
#parScan = nll_scan(model, input = 'data', n = 1, range = [-3.0, 3.0], parameter='pdfas', signal_process_groups = signal_process_groups, options = options)
#print parScan
#for p in parScan['']:
#    p.write_txt('comb_ex1or2btags_hardsoft_pdfasscan.txt')
#
#parScan = nll_scan(model, input = 'data', n = 1, range = [-3.0, 3.0], parameter='btag', signal_process_groups = signal_process_groups, options = options)
#print parScan
#for p in parScan['']:
#    p.write_txt('comb_ex1or2btags_hardsoft_btagscan.txt')
#
#parScan = nll_scan(model, input = 'data', n = 1, range = [-3.0, 3.0], parameter='mtag', signal_process_groups = signal_process_groups, options = options)
#print parScan
#for p in parScan['']:
#    p.write_txt('comb_ex1or2btags_hardsoft_mtagscan.txt')
#
#parScan = nll_scan(model, input = 'data', n = 1, range = [-3.0, 3.0], parameter='q2scale', signal_process_groups = signal_process_groups, options = options)
#print parScan
#for p in parScan['']:
#    p.write_txt('comb_ex1or2btags_hardsoft_q2scalescan.txt')
#
#parScan = nll_scan(model, input = 'data', n = 1, range = [-3.0, 3.0], parameter='jes', signal_process_groups = signal_process_groups, options = options)
#print parScan
#for p in parScan['']:
#    p.write_txt('comb_ex1or2btags_hardsoft_jesscan.txt')

c = open('comb_ex1or2btags_hardsoft_cov.json', 'w')
c.write(str(mle(model, input = 'data', n=1, with_covariance=True, signal_process_groups = signal_process_groups, options = options)['']['__cov']).replace('\n','').replace('[array(','').replace(')]',''))
c.close()
d = open('comb_ex1or2btags_hardsoft_names.json', 'w')
json.dump([p for p in model.get_parameters([])], d)
d.close()


#histogram_specs = {'jer': [(30, -3.0, 3.0)], 'jes': [(30, -3.0, 3.0)], 'mulepeff': [(30, -3.0, 3.0)], 'btag': [(30, -3.0, 3.0)], 'PU': [(30, -3.0, 3.0)], 'muhlteff': [(30, -3.0, 3.0)], 'q2scale': [(30, -3.0, 3.0)], 'zerotagshape': [(30, -3.0, 3.0)], 'beta_signal': [(30, -3.0, 3.0)], 'lumi': [(30, -3.0, 3.0)], 'hf': [(30, -3.0, 3.0)], 'matching': [(30, -3.0, 3.0)], 'shape': [30, -3.0, 3.0]}
#histogram_specs = {'mulepeff': [(30, -3.0, 3.0)], 'muhlteff': [(30, -3.0, 3.0)], 'elepeff': [(30, -3.0, 3.0)], 'ehlteff': [(30, -3.0, 3.0)], 'q2scale': [(30, -3.0, 3.0)], 'beta_signal': [(30, -3.0, 3.0)], 'lumi': [(30, -3.0, 3.0)], 'murmuf': [30, -3.0, 3.0], 'toppt': [30, -3.0, 3.0], 'wjets_rate': [30, -3.0, 3.0], 'ttbar_rate': [30, -3.0, 3.0], 'pdfas': [30, -3.0, 3.0], 'sigpdfas': [30, -3.0, 3.0], 'pileup': [30, -3.0, 3.0]}

#posteriors(model, histogram_specs, signal_processes = [['wp900']])
#fit_posterior = posteriors(model, histogram_specs)

# model_summary, bayesian_limits, and cls_limits also write their results to the 'report' object
# which we can ask to write its results as html page to a certain directory. Use an existing, empty
# directory and point your web browser to it.
#report.write_html('htmlout_comb_ex1or2btags')

# After running theta-auto, you probably want to delete the 'analysis' directory which
# contains intermediate results. Keeping it avoids re-running theta unnecessarily for unchanged configurations
# (e.g., because you just want to change the plot). However, this directory can grow very large over time
