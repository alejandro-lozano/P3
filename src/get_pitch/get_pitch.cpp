/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>

#include "wavfile_mono.h"
#include "pitch_analyzer.h"

#include "docopt.h"

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

static const char USAGE[] = R"(
get_pitch - Pitch Detector 

Usage:
    get_pitch [options] <input-wav> <output-txt>
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -0 FLOAT, --umb_pot=FLOAT      Umbral potencia en dB [default: -18]
    -1 FLOAT, --umb_R1=FLOAT      Umbral para autocorrelación r1norm [default: 0.5]
    -2 FLOAT, --umb_RMax=FLOAT      Umbral para autocorrelación rmaxnorm [default: 0.4]  
    -c FLOAT, --clipping=FLOAT    Umbral para center clipping [default: 0.0001]
    -z FLOAT, --umb_ZRC=FLOAT    Umbral para zero-crossing [default: 160]
    -h, --help                Show this screen
    --version                 Show the version of the project

Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the detection:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
)";

int main(int argc, const char *argv[]) {
	/// \TODO 
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program.
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string

	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();
  float umb_pot = stof(args["--umb_pot"].asString());
  float umb_R1 = stof(args["--umb_R1"].asString());
  float umb_RMax = stof(args["--umb_RMax"].asString());
  float clipping = stof(args["--clipping"].asString());
  float umb_ZRC = stof(args["--umb_ZRC"].asString());



  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;

  // Define analyzer
  PitchAnalyzer analyzer(n_len, rate, umb_pot, umb_R1, umb_RMax, umb_ZRC, PitchAnalyzer::RECT, 50, 500);

  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
  unsigned int i;
  for (i = 0; i < x.size(); i++){
    if (x[i] > clipping)
      x[i] = x[i] - clipping;
    else if (x[i] < -1 * clipping)
      x[i] = x[i] + clipping;
    else
      x[i] = 0;
  }
  
  // Iterate for each frame and save values in f0 vector
  vector<float>::iterator iX;
  vector<float> f0;
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    float f = analyzer(iX, iX + n_len);
    f0.push_back(f);
  }

  /// \TODO
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.
  for (i = 1; i < f0.size()-1; i++){
    vector<float> aux;
    aux.push_back(f0[i-1]);
    aux.push_back(f0[i]);
    aux.push_back(f0[i+1]);
    std::sort (aux.begin(), aux.end());
    f0[i] = aux[1];
  }

  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = f0.begin(); iX != f0.end(); ++iX) 
    os << *iX << '\n';
  os << 0 << '\n';//pitch at t=Dur

  return 0;
}
