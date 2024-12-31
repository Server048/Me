#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <thread>
#include <chrono>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace web;
using namespace http;
using namespace http::experimental::listener;
using namespace utility;
using namespace concurrency;

// Ganti dengan port yang Anda inginkan
const int port = 3000;

void send_message(const std::string& message) {
  std::cout << "Pesan yang diterima: " << message << std::endl;
}

void handle_request(http_request request) {
  // Mendapatkan request body
  auto body = request.extract_string().get();
  json request_json = json::parse(body);

  // Memeriksa apakah pesan ada dalam request body
  if (!request_json.contains("message")) {
    request.reply(status_codes::BadRequest, U("Pesan tidak ditemukan."));
    return;
  }

  // Mendapatkan pesan dari request body
  string message = request_json["message"].get<string>(); // Simpan pesan ke variabel lokal

  // Menampilkan pesan di konsol
  cout << "Pesan User: " << message << endl;

  // Menjalankan perintah `echo` di terminal
  string command = "echo " + message;
  system(command.c_str());

  // Periksa apakah message tidak kosong
  if (!message.empty()) {
    // Panggil fungsi send_message untuk menampilkan pesan
    send_message(message);
  } else {
    std::cout << "Pesan kosong, tidak akan ditampilkan." << std::endl;
  }

  // Mengirim respons ke client
  request.reply(status_codes::OK, U("Pesan diterima dan di-echo ke terminal."));
}

int main() {
  // Membuat HTTP listener
  http_listener listener(U("http://localhost:"));
  listener.open().wait();
  cout << "Server listening on port " << port << endl;

  // Menangani permintaan POST ke `/discord`
  listener.support(methods::POST, U("/discord"), handle_request);

  // Menjalankan listener secara asynchronous
  while (true) {
    this_thread::sleep_for(chrono::milliseconds(100));
  }

  return 0;
}
