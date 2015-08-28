// Copyright 2015 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Example using the polyblep class.

#include "dsp/oscillator.h"
#include "test/wav_writer.h"

using namespace stmlib;

const size_t kSampleRate = 96000;
const size_t kAudioBlockSize = 24;
const size_t kSampleDuration = 10;

float Mix(float s, float a, float b) {
    return (1-s)*a + s*b;
}

int main(void) {
  Oscillator osc;
  osc.Init();
  
  WavWriter wav_writer(1, kSampleRate, kSampleDuration);
  wav_writer.Open("sample.wav");

  size_t num_blocks = kSampleDuration * kSampleRate / kAudioBlockSize;
  for (size_t i = 0; i < num_blocks; ++i) {
    float out[kAudioBlockSize];
    osc.Render<true>(
        100.00f / 96000.0f,  // master
        100.00f * (0.5f + 4.0f * float(i) / num_blocks) / 96000.0f,  // slave
        0.5f,  // pulse-width
        0.9f,  // shape: 0.0 for saw, 1.0 for square
        out, kAudioBlockSize);
    wav_writer.Write(out, kAudioBlockSize);
  }

  WavWriter wav_writer2(1, kSampleRate, kSampleDuration);
  wav_writer2.Open("sweep.wav");

  const int n = kSampleRate * kSampleDuration;
  for (size_t i = 0; i < n; ++i) {
    float out;
    osc.Render<false>(
        Mix(float(i)/n, 100.00f, 15000.f) / kSampleRate,  // master
        0.5f,  // pulse-width
        0.0f,  // shape: 0.0 for saw, 1.0 for square
        &out, 1);
    wav_writer2.Write(&out, 1);
  }

}
