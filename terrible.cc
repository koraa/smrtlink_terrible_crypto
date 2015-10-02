#include <cstdint>
#include <vector>
#include <utility>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

typedef unsigned char byte;
typedef vector<byte> bytes;

/// Generate the keystream
///
/// I don't know exactly what this does, but to me this
/// essentially looks like a PRNG (probably non crypto-grade)
/// that takes the key as a seed.
///
/// Known properties:
/// * The key has to be made up of 256 unsigned bytes
/// * The PRNG generates chaotic values by indirectly
///   accessing key elements by taking another element as
///   index taking that as index again and doing some
///   summing
/// * It also generates chaos by swapping keys around in the
///   key (that's why we take a copy of the key)
/// * The resulting keystream is limited to bytes that also
///   occur in the key
/// * The keystreams symbol frequencies should be quite
///   similar to the keys
/// * I am pretty sure that the keystream leaks more
///   information about the key, but I haven't found out how
///   yet
vector<byte> generate_keystream(bytes key /* copy */, size_t len) {
  vector<byte> out(len);

  byte q = 0;
  for (size_t ix = 0; ix < len; ix++) {

    // variable meanings unknown

    // Values: 1, 2, 3, ..., 255, 1, ...Q
    byte h = (ix + 1) % 256;
    // Values: key[1], key[1] + key[2], ...
    // as subject to the swapping below
    q = (q + key[h]) % 256;

    swap(key[h], key[q]);

    byte w = (key[h] + key[q]) % 256;

    out[ix] = key[ w ];
  }

  return out;
}

/// XOR two entire vectors of bytes
bytes& operator^=(bytes &a, const bytes &b) {
  for (size_t ix = 0; ix < a.size(); ix++)
    a[ix] ^= b[ix];
  return a;
}

/// encrypt & decrypt;
///
/// This simply generates the static keystream and XORs the
/// input with the keystream.
/// This both encrypts and decrypts because double-xor
/// results in the input `A = xor(xor(A, K), K)`
///
/// No padding to conceal size, no salting because the
/// original algorithm didn't do so either.
vector<byte> crypt(const bytes &key, const bytes &text) {
bytes ks = generate_keystream(key, text.size());
ks ^= text;
return ks;
}

template<typename Str>
bytes read_stream(Str &s) {
bytes buf(2048);
size_t used = 0;

while (true) {
  size_t do_read = buf.size() - used;
  s.read((char*)&buf[0], do_read);
  used += s.gcount();

  if (s.eof())
    break;
  else if ((size_t)s.gcount() < do_read)
    continue;
  else {
    buf.resize( buf.size() * 2);
  }
}

buf.resize(used);
return buf;
}

bytes read_file(const string &name) {
  fstream str(name, ios::binary | ios::in);
  return read_stream(str);
}


template<typename Str>
void write_stream(Str &s, const bytes &buf) {
  s.write((char*)&buf[0], buf.size());
}

void usage() {
  cerr << "USAGE: "
    << "\n  (1) terrible crypt keyfile <plaintext >cyphertext"
    << "\n  (2) terrible keystream keyfile length > keystream_file"
    << "\n  (3) terrible xor file_a file_b > result\n";
  exit(1);
}

int main(int argc, char **argv) {
  if (argc < 2) usage();
  string command = argv[1];


  if (command == "crypt") {
    if (argc < 3) usage();
    bytes key = read_file(argv[2]);
    bytes text = read_stream(cin);

    write_stream(cout, crypt(key, text));

  } else if (command == "keystream") {
    if (argc < 4) usage();
    bytes key = read_file(argv[2]);
    auto len = atoll(argv[3]);

    write_stream(cout, generate_keystream(key, len));

  } else if (command == "xor") {
    if (argc < 4) usage();
    bytes buf = read_file(argv[2]);
    buf ^= read_file(argv[3]);

    write_stream(cout, buf);

  } else {
    usage();
  }

  return 0;
}
