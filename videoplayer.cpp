// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "videoplayer.h"

#include <QBoxLayout>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMediaPlayer>
#include <QPushButton>
#include <QSizePolicy>
#include <QSlider>
#include <QStandardPaths>
#include <QString>
#include <QStyle>
#include <QVideoWidget>

VideoPlayer::VideoPlayer(QWidget *parent) : QWidget(parent)
{
    m_mediaPlayer = new QMediaPlayer(this); // 播放器组件
    QVideoWidget *videoWidget = new QVideoWidget;

    QAbstractButton *openButton = new QPushButton(tr("Open...")); // 打开文件按钮
    connect(openButton, &QAbstractButton::clicked, this, &VideoPlayer::openFile);

    m_playButton = new QPushButton; // 播放键
    m_playButton->setEnabled(false);
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(m_playButton, &QAbstractButton::clicked, this, &VideoPlayer::play); // 连接信号槽，点击播放键->视频播放

    m_positionSlider = new QSlider(Qt::Horizontal); // 进度条
    m_positionSlider->setRange(0, 0);

    connect(m_positionSlider, &QAbstractSlider::sliderMoved, this, &VideoPlayer::setPosition); // 连接信号槽，拖动滑动条->设置播放进度

    m_errorLabel = new QLabel; // 报错文本
    m_errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QBoxLayout *controlLayout = new QHBoxLayout; // 弄个盒子把下面的装起来
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->addWidget(openButton);
    controlLayout->addWidget(m_playButton);
    controlLayout->addWidget(m_positionSlider);

    QBoxLayout *layout = new QVBoxLayout; // 弄个大盒子全装起来
    layout->addWidget(videoWidget);
    layout->addLayout(controlLayout);
    layout->addWidget(m_errorLabel);

    setLayout(layout);

    m_mediaPlayer->setVideoOutput(videoWidget);
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this,
            &VideoPlayer::mediaStateChanged);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
    connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);
    connect(m_mediaPlayer, &QMediaPlayer::errorChanged, this, &VideoPlayer::handleError);
}

VideoPlayer::~VideoPlayer() { }

void VideoPlayer::openFile() // 打开指定的文件
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Movie"));
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation)
                                    .value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        setUrl(fileDialog.selectedUrls().constFirst());
}

void VideoPlayer::setUrl(const QUrl &url) // 打开在线文件
{
    m_errorLabel->setText(QString());
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    m_mediaPlayer->setSource(url);
    m_playButton->setEnabled(true);
}

void VideoPlayer::play() // 播放
{
    switch (m_mediaPlayer->playbackState()) {
    case QMediaPlayer::PlayingState:
        m_mediaPlayer->pause();
        break;
    default:
        m_mediaPlayer->play();
        break;
    }
}

void VideoPlayer::mediaStateChanged(QMediaPlayer::PlaybackState state) // 视频状态更改
{
    switch (state) {
    case QMediaPlayer::PlayingState:
        m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void VideoPlayer::positionChanged(qint64 position) // 调整位置后
{
    m_positionSlider->setValue(position);
}

void VideoPlayer::durationChanged(qint64 duration) // 视频进度变动
{
    m_positionSlider->setRange(0, duration);
}

void VideoPlayer::setPosition(int position) // 设置视频进度
{
    m_mediaPlayer->setPosition(position);
}

void VideoPlayer::handleError() // 报错
{
    if (m_mediaPlayer->error() == QMediaPlayer::NoError)
        return;

    m_playButton->setEnabled(false);
    const QString errorString = m_mediaPlayer->errorString();
    QString message = "Error: ";
    if (errorString.isEmpty())
        message += " #" + QString::number(int(m_mediaPlayer->error()));
    else
        message += errorString;
    m_errorLabel->setText(message);
}

#include "moc_videoplayer.cpp"
