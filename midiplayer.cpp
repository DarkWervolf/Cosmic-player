#include "midiplayer.h"
#include <QDebug>
#include <QDateTime>

MidiPlayer::MidiPlayer(QMidiOut* out) : midi_file(0), midi_out(out), state(STOPPED), eventNumber(0), number(0)
{
}
MidiPlayer::~MidiPlayer()
{
    stop();
    midi_file->clear();
    delete midi_file;
    midi_out->stopAll();
    delete midi_out;
}

void MidiPlayer::run()
{
    if (!midi_file) {
        return;
    }
    QElapsedTimer t;
    t.start();
    QList<QMidiEvent*> events = midi_file->events();
    eventNumber = number;
    state = PLAYING;
    qDebug() << eventNumber;
    bool paused = false;
    qint64 posTimer = 0;
    qint64 pauseTime = 0;
    for (int i = eventNumber; i < events.size() ; i++){
        QMidiEvent* e = events[i];
        if (state != PLAYING) {
            paused = true;
            posTimer = t.elapsed();
            pauseTime += posTimer;
            while (state != PLAYING) {
                msleep(500);
            }
            t.restart();
        }
        else {
            posTimer = t.elapsed();
        }
        if (e->type() != QMidiEvent::Meta) {
            qDebug() << i;
            qint64 event_time = midi_file->timeFromTick(e->tick()) * 1000;

            qint32 waitTime = event_time - (posTimer + pauseTime);
            if (waitTime > 0 && !paused) {
                msleep(waitTime);
            }
            if (e->type() == QMidiEvent::SysEx) {
                // TODO: sysex
            } else {
                qint32 message = e->message();
                midi_out->sendMsg(message);
                emit eventChanged(++eventNumber);
            }
        }
        paused = false;
    }
    
    midi_out->disconnect();
    delete midi_file;
}

void MidiPlayer::stop()
{
    state = STOPPED;
    number = 0;
}

void MidiPlayer::setPosition(int pos)
{
    state = PAUSED;
    number = pos;
    eventNumber = pos;
    state = PLAYING;
}

void MidiPlayer::pause()
{
        //number = eventNumber;
        state = PAUSED;

}

void MidiPlayer::setMidiFile(QMidiFile* file)
{
    midi_file = file;
}

