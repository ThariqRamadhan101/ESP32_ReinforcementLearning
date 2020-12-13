#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "data.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
// Create an area of memory to use for input, output, and intermediate arrays.
// Finding the minimum value for your model may require some trial and error.
constexpr int kTensorArenaSize = 100 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}

// The name of this function is important for Arduino compatibility.

void setup() {
  Serial.begin(115200);
  // Configure record. Global Google style should be avoided
  // or static due to life time uncertainty,
  // but it has a trivial destructor, okay.
  // NOLINTNEXTLINE (global variables at run time)
  // Map the model into a usable data structure.
  // This does not imply any copying or analysis,
  // is a very light operation.

  model = tflite::GetModel(mi_data);

  // This state in all operational implementations we need.
  // NOLINTNEXTLINE (global variables at run time)
  static tflite::ops::micro::AllOpsResolver resolver;

  // Create an interpreter to run the model.
  static tflite::MicroInterpreter static_interpreter( model,
      resolver,
      tensor_arena,
      kTensorArenaSize,
      error_reporter);
  interpreter = &static_interpreter;

  // Assign memory of tensor_arena for the tensors of the model.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();

  // Get pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);

  // Calculate an x value to feed the model.
  int features = 25;
  for (int u = 0; u < features; u++) {
    // Tempatkan nilai yang dihitung pada tensor input model
    input->data.f[u] = test[u];
  }

  // Jalankan model pada masukan ini dan verifikasikan berhasil
  TfLiteStatus invoke_status = interpreter->Invoke();


  // Obtenga el valor de salida del tensor
  float left = output->data.f[0];
  float up = output->data.f[1];
  float right = output->data.f[2];
  float down = output->data.f[3];
  Serial.print("left : ");
  Serial.println(left,5);
  Serial.print("up : ");
  Serial.println(up, 5);
  Serial.print("right : ");
  Serial.println(right, 5);
  Serial.print("down : ");
  Serial.println(down, 5);


}

void loop() {

}

//by sandro ormeño edited by thariq
