#include "midiplayer.h"
#include <QDebug>
#include <QDateTime>

MidiPlayer::MidiPlayer(QMidiOut* out) : midi_file(0), midi_out(out), state(STOPPED), evNumber(100)
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
    state = PLAYING;
    qDebug() << evNumber;
    bool paused = false;
    qint64 posTimer = 0;
    qint64 pauseTime = 0;
    for (; evNumber < events.size(); evNumber++){
        qDebug() << "evNum" << evNumber << state << events.size();
        QMidiEvent* e = events[evNumber];
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
                emit eventChanged(evNumber);
            }
        }
        paused = false;
    }
    
    midi_out->disconnect();
    delete midi_file;
}

void MidiPlayer::stop()
{
    midi_out->stopAll();
    state = STOPPED;
    //evNumber = 0;
}

void MidiPlayer::setPosition(int pos)
{
    qDebug() << "setPosition() " << pos;
    evNumber = pos;
}

void MidiPlayer::pause()
{
    state = PAUSED;
}

void MidiPlayer::setMidiFile(QMidiFile* file)
{
    midi_file = file;
}

