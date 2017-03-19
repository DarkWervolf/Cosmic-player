#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include <QMidiFile.h>
#include <QMidiOut.h>
#include <QThread>
#include <QElapsedTimer>
#include <QDebug>

class MidiPlayer : public QThread
{
    Q_OBJECT
    friend class MainWindow;
public:
    MidiPlayer(QMidiOut* out);
    ~MidiPlayer();
    enum states {STOPPED, PLAYING, PAUSED};
    int state;
    void stop();
    void pause();
    void setMidiFile(QMidiFile* file);
    int number;
private:
    QMidiFile* midi_file;
    QMidiOut* midi_out;
    int eventNumber;
protected:
    void run();
signals:
    void eventChanged(int);
};

#endif // MIDIPLAYER_H
