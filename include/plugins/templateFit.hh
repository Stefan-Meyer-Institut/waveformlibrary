#ifndef __TEMPLATEFIT_HH__
#define __TEMPLATEFIT_HH__
#ifdef __HAVEROOT

#include<string>

#include"SMIWaveformAnalyzerPluginSystem.hh"
#include"plugins/fitBase.hh"
namespace plugin {

  //! plugin for template fitting of waveforms
  /*!
    this plugins takes a pre calculated waveform template and tries
    to fit it into the waveform data. This is done by using the minuit 2 
    minimiser. The template has to be provided as a text file containing 
    the time axis as first column and the waveform as second column. To get
    good results it is important to have the waveform tamplate normalised to 1

    After fitting the data the plugin uses an F-test to compare against a 
    straight line to discriminate against empty channels. The probability for
    NOT identifying a signal close to the zero signal is 0.5 %. This means 
    the plugin requires a p<0.005 to reject the null hypothesis that there is a
    valid signal in the channel.
   */
  class templateFit : public fitBase {
  public:
    //! constructor
    /*!
      reads the waveform template and prepares it for fitting

      @param templatePath path to the text file that contains the template
      @param fitLow low bound of the fit on x axis
      @param fitHigh high bound of the fit on x axis
     */
    templateFit(std::string templatePath, double fitLow, double fitHigh):
      fitBase(fitLow, fitHigh, 3), currentData(NULL)
    {
      readWaveformTemplate(templatePath);
    }

    //! operator to use the plugin
    /*!
      this function takes the @ref waveform and performs the fit and F-test
      discrimination in the following order:
      -# guess starting parameters for fit
      -# perform actal fitting
      -# perform F-test for zero discrimination
      -# produce output depending on F-test and convergence of fit

      @param wave waveform to fit
      @return true if fit converged and F-test says a signal is present
     */
    virtual bool operator()(WaveForm &wave);

  protected:
    //! error function, returning \f$\chi^2\f$ of fit described by par
    /*!
      calculate the template based on the fitting parameters par and determin 
      \f$\chi^2\f$ by summing over the mean square errors

      @param par par[0]-time offset, par[1]-amplitude, par[2]-baseline
      @return \f$\chi^2\f$
     */
    virtual double errorFunction(const Double_t *par); 

  private:
    //! read waveform template from par
    /*!
      @param templatePath path to text file containin a waveform template
      @return true if the template can be read
     */
    bool readWaveformTemplate(std::string &templatePath);

    //! guess the statin parameters for the fit
    /*!
      - baseline is guessed by avaraging over the first 20 bins in the signal
      - amplitude is the maximum of the signal minus baseline
      - timeoffset if position of maximum minus 6.5 ns

      @param par array with the starting parameters
      @param wave waveform to analyse
     */
    void guessStartingParameters(Double_t *par, WaveForm &wave);

    //! fit the data by using parameters stored in par
    /*!
      if the fit converges, this includes the case when the errors are not 
      fully relyable, the resulting parameters are stored within par.

      @param par starting parameters and fitting results
      @return true if the fit converged
     */
    bool fitData(Double_t *par);

    //! perform F-test to dicriminate against empty channels
    /*!
      -# calculate avarage of the signal within the bounds (alt. hypothesis)
      -# calculate \f$\chi^2\f$ for straight line
      -# calculate \f$\chi^2\f$ for template fit line (also within bounds)
      -# determin F: 
      \f[
         F = \frac{ \frac{\chi^2_\mathsf{line} - \chi^2_\mathsf{template}}{\mathsf{NDF}_\mathsf{template} - \mathsf{NDF}_\mathsf{line}} }{ \frac{\chi^2_\mathsf{template}}{\mathsf{N}_\mathsf{sample}-\mathsf{NDF}_\mathsf{template}} }
      \f]

      \f[
      \mathsf{NDF}_\mathsf{template}\, = \, 3 \,\,\,\,\,
      \mathsf{NDF}_\mathsf{line}\, = \, 1
      \f]
      -# calculate p value from the F distribution CDF
      
      @param par parameters of template fit
      @return true if p > 0.005
     */
    bool makeFTest(Double_t *par);

    //! store fit without baseline in wave and store parameters in dictonary
    /*!
      store time offset in 'LEtime' and amplitude in 'A', if F-test failed or
      the fit didn't converge store -999999 in 'A'.

      @param converged  did the fit converge?
      @param fTest is a valid signal present
      @param par fitting parameters
      @wave waveform class, is modified to hold the fit function and the results are stored within SMIWave::result
     */
    bool produceOutput(bool converged, bool fTest, const Double_t *par, WaveForm &wave);

    WaveForm  templFunc;   //!< stores the wavefrom template
    WaveForm *currentData; //!< pointer to the current data, required for fitting
  };

}
#endif
#endif
