#include <cpprpc/stub/ServiceStubGenerator.hpp>
#include <cpprpc/stub/StubGenerator.hpp>
#include <cpprpc/Exception.hpp>
#include <cppjson/FileReadStream.hpp>
#include <cppjson/Document.hpp>
#include <unistd.h>

static void usage() {
    fprintf(stderr, "usage: stub_generator <-c/s> [-o] [-i input]\n");
    exit(1);
}

static void writeToFile(CPPRPC::StubGenerator& generator, bool outputToFile) {
    FILE* output;
    if (!outputToFile) {
        output = stdout;
    } else {
        std::string outputFileName = generator.genStubClassName() + ".hpp";
        output = fopen(outputFileName.c_str(), "w");
        if (output == nullptr) {
            perror("error");
            exit(1);
        }
    }

    auto stubString = generator.genStub();
    fputs(stubString.c_str(), output);
}

static std::unique_ptr<CPPRPC::StubGenerator> makeGenerator(bool serverSide, cppjson::Value& proto) {
    if (serverSide) {
        return std::make_unique<CPPRPC::ServiceStubGenerator>(proto);
    }
    else {
        // return std::make_unique<CPPRPC::ClientStubGenerator>(proto);
        return nullptr;
    }
}

static void genStub(FILE* input, bool serverSide, bool outputToFile) {
    cppjson::FileReadStream is(input);
    cppjson::Document proto;
    auto err = proto.parseStream(is);
    if (err != cppjson::PARSE_OK) {
        fprintf(stderr, "%s\n", cppjson::ParseErrorStr(err));
        exit(1);
    }

    try {
        auto generator = makeGenerator(serverSide, proto);
        writeToFile(*generator, outputToFile);
    }
    catch (CPPRPC::StubException& e) {
        fprintf(stderr, "input error: %s\n", e.what());
        exit(1);
    }
}


int main(int argc, char** argv) {
    bool serverSide = false;
    bool clientSide = false;
    bool outputToFile = false;
    const char* inputFileName = nullptr;

    int opt;
    while((opt = getopt(argc, argv, "csi:o")) != -1) {
        switch(opt) {
            case 'c':
                clientSide = true;
            case 's':
                serverSide = true;
            case 'o':
                outputToFile = true;
            case 'i':
                inputFileName = optarg;
                break;
            default:
                fprintf(stderr, "unknown flag %c\n", opt);
                usage();
        }
    }
    if (!serverSide && !clientSide) {
        serverSide = clientSide = true;
    }
    FILE* input = stdin;
    if (inputFileName != nullptr) {
        input = fopen(inputFileName, "r");
        if (input == nullptr) {
            perror("error");
            exit(1);
        }
    }

    try {
        if (serverSide) {
            genStub(input, true, outputToFile);
            rewind(input);
        }
        if (clientSide) {
            genStub(input, false, outputToFile);
        }
    } catch(CPPRPC::StubException& e) {
        fprintf(stderr, "input error: %s\n", e.what());
        exit(1);
    }
    return 0;
}
