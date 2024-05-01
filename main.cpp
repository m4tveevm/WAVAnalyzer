#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <ncurses.h>
#include <fftw3.h> // without this lib project worked so slow :(

struct WAVHeader {
    char chunkID[4];
    uint32_t chunkSize;
    char format[4];
    char subchunk1ID[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2ID[4];
    uint32_t subchunk2Size;
};

void readWAVHeader(std::ifstream &file, WAVHeader &header) {
    if (!file.read((char*)&header, sizeof(WAVHeader))) {
        std::cerr << "Could not read WAV header" << std::endl;
        exit(1);
    }
}

void displayHeaderInfo(const WAVHeader &header) {
    clear();
    printw("File Header Information:\n");
    printw("Sample Rate: %d Hz\n", header.sampleRate);
    printw("Channels: %d\n", header.numChannels);
    printw("Bits per Sample: %d\n", header.bitsPerSample);
    printw("\nPress SPACE to start analysis, 'q' to quit.\n");
    refresh();
}

void performFFT(const std::vector<int16_t> &input, std::vector<double> &magnitudes) {
    size_t n = input.size();
    fftw_complex *in, *out;
    fftw_plan p;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
    p = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    for (size_t i = 0; i < n; ++i) {
        in[i][0] = input[i];
        in[i][1] = 0.0;
    }

    fftw_execute(p);

    magnitudes.resize(n / 2);
    for (size_t i = 0; i < n / 2; ++i) {
        magnitudes[i] = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
    }

    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);
}

void displayHistogram(const std::vector<double> &frequencies, const std::vector<double> &magnitudes, int currentSecond) {
    clear();
    printw("========================Histogram============================\n");
    printw("| Current Second: %d\n", currentSecond);

    double maxMagnitude = *max_element(magnitudes.begin(), magnitudes.end());
    const int maxBarLength = 50;

    for (size_t i = 0; i < frequencies.size(); ++i) {
        double normalizedMagnitude = (magnitudes[i] / maxMagnitude) * maxBarLength;
        printw("| Frequency %5g Hz: ", frequencies[i]);
        for (int j = 0; j < normalizedMagnitude; ++j) {
            printw("*");
        }
        printw("\n");
    }

    printw("============================================================\n");
    refresh();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <WAV file>\n";
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << argv[1] << std::endl;
        return 1;
    }

    WAVHeader header;
    readWAVHeader(file, header);

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    displayHeaderInfo(header);
    int ch = getch();
    while (ch != 'q') {
        if (ch == ' ') {
            int currentSecond = 0;
            do {
                std::vector<double> frequencies = {50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000};
                std::vector<int16_t> data(header.sampleRate * header.numChannels);
                std::vector<double> magnitudes;

                file.seekg(sizeof(WAVHeader) + currentSecond * header.sampleRate * header.blockAlign, std::ios::beg);
                if (file.read((char*)data.data(), data.size() * sizeof(int16_t))) {
                    performFFT(data, magnitudes);
                    displayHistogram(frequencies, magnitudes, currentSecond);
                }

                ch = getch();
                if (ch == KEY_RIGHT) currentSecond++;
                if (ch == KEY_LEFT && currentSecond > 0) currentSecond--;
            } while (ch != 'q' && ch != ' ');
        }
        ch = getch();
    }

    endwin();
    file.close();
    return 0;
}
