#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaPlayerControl>
#include <QDebug>
#include <QFont>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), midiPlayer(0), check(0), paused(false), checkPosition(false), pos(0), state(STOPPED), evNumber(0)
{
    //midiFilename = "D:/cpp/Midi-master/build-Midi-Desktop_Qt_5_5_1_MinGW_32bit-Debug/MeinHerzBrennt.mid";
    ui->setupUi(this);
    connect(ui->openMidiButton, SIGNAL(clicked(bool)), this, SLOT(openMidiFileDialog()));
    connect(ui->playMidiButton, SIGNAL(clicked(bool)), this, SLOT(playMidi()));
    connect(ui->openAudioButton, SIGNAL(clicked(bool)), this, SLOT(openAudioFileDialog()));
    connect(ui->playAudioButton, SIGNAL(clicked(bool)), this, SLOT(playAudio()));
    connect(ui->stopButton, SIGNAL(clicked(bool)), this, SLOT(stopAudio()));
    connect(ui->pauseAudioButton, SIGNAL(clicked(bool)), this, SLOT(pauseAudio()));
    connect(ui->pauseMidiButton, SIGNAL(clicked(bool)), this, SLOT(pauseMidi()));
    connect(ui->stopMidiButton, SIGNAL(clicked(bool)), this, SLOT(stopMidi()));

    audioPlayer = new QMediaPlayer;
    connect(audioPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(moveAudioSlider(qint64)));
    connect(ui->fileSlider, SIGNAL(sliderPressed()), this, SLOT(setPositionAudio(int)));
    connect(ui->fileSlider, SIGNAL(sliderMoved(int)), this, SLOT(setPositionAudio(int)));

    playlist = new QMediaPlaylist;

    midi_out = new QMidiOut();
    midi_out->connect(QMidiOut::devices().firstKey());
    midiPlayer = new MidiPlayer(midi_out);
    //connect(midiPlayer, SIGNAL(finished()), midiPlayer, SLOT(deleteLater()));
    connect(midiPlayer, SIGNAL(eventChanged(int)), this, SLOT(moveMidiSlider(int)));
    //connect(ui->fileSlider, SIGNAL(sliderPressed()), this, SLOT(setPositionMidi(int)));
    connect(ui->fileSlider, SIGNAL(sliderMoved(int)), this, SLOT(setPositionMidi(int)));

}

MainWindow::~MainWindow()
{
    delete audioPlayer;
    delete ui;
    delete playlist;
}

void MainWindow::openMidiFileDialog()
{
    midiFilename = QFileDialog::getOpenFileName(this, "Open Midi Files", "", "MIDI Files (*.midi *.mid)");
    ui->filenameEdit->setText(midiFilename);
}

void MainWindow::playMidi()
{
    stopAudio();
    //stopMidi();
    if (!midiFilename.isEmpty()) {
        midiFile = new QMidiFile;
        midiFile->load(midiFilename);
        qDebug() << midiFile->events().count();
        ui->fileSlider->setRange(0, midiFile->events().count());
        ui->fileSlider->setPageStep(1);
        ui->fileSlider->setTickInterval(midiFile->events().count() / 50);
        ui->fileSlider->setTickPosition(QSlider::TicksBelow);
        ui->fileSlider->setValue(0);
        midiPlayer->setMidiFile(midiFile);
        midiPlayer->start();
   }
   // midiPlayer->setPosition(5000);
}

void MainWindow::stopMidi()
{
    midiPlayer->state = STOPPED;
    midiPlayer->stop();
    ui->fileSlider->setValue(0);
}

void MainWindow::openAudioFileDialog()
{
    audioFilename = QFileDialog::getOpenFileName(this, "Open Audio Files", "", "Audio Files (*.mp3 *.wav)");
    ui->filenameEdit->setText(audioFilename);
    playlist->addMedia(QUrl::fromLocalFile(audioFilename));
}

void MainWindow::playAudio()
{
    stopAudio();
    stopMidi();
    if(!audioFilename.isEmpty()){
        audioPlayer->setPlaylist(playlist);
        playlist->setCurrentIndex(1);
        audioPlayer->setPlaylist(playlist);
        audioPlayer->play();
    }
}

void MainWindow::stopAudio()
{
    audioPlayer->stop();
}

void MainWindow::pauseAudio()
{
    if (audioPlayer->state() == QMediaPlayer::PlayingState){
        audioPlayer->pause();
    }
    else if (audioPlayer->state() == QMediaPlayer::PausedState){
        audioPlayer->play();
    }
}

void MainWindow::moveMidiSlider(int value)
{
    qDebug() << "moveSlider() " <<  value;
    if(checkPosition == true){
        return;
    }
    else{
        ui->fileSlider->setValue(value);
    }
}

void MainWindow::moveAudioSlider(qint64 value)
{
    ui->fileSlider->setMaximum(audioPlayer->duration());
    ui->fileSlider->setValue(value);
}

void MainWindow::setPositionAudio(int pos)
{
    audioPlayer->setPosition(pos);
}

void MainWindow::setPositionMidi(int pos)
{
    qDebug() << "setPosMidi() " << pos;
    checkPosition = true;
    midiPlayer->setPosition(pos);
    checkPosition = false;
}

void MainWindow::pauseMidi()
{
    if (midiPlayer->state == PLAYING){
        state = PAUSED;
        midi_out->stopAll();
        midiPlayer->state = PAUSED;
        midiPlayer->pause();
    }
    else if(state == PAUSED){
        state = PLAYING;
        midiPlayer->state = PLAYING;
        //midiPlayer->number = evNumber;
        /*
            qDebug() << "working";
            midi_out = new QMidiOut();
            midi_out->connect(QMidiOut::devices().firstKey());
            midiPlayer = new MidiPlayer(midi_out);
            connect(midiPlayer, SIGNAL(eventChanged(int)), this, SLOT(moveMidiSlider(int)));
            midiFile = new QMidiFile;
            midiFile->load(midiFilename);
            midiPlayer->setMidiFile(midiFile);
            midiPlayer->number = evNumber;
            midiPlayer->start();
            state = PLAYING;
            */
    }
}
