#ifndef _HTTP_CLIENT_HPP
#define _HTTP_CLIENT_HPP

#include <QObject>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QEventLoop>

#include "httprequest.hpp"
#include "httpresponse.hpp"

namespace Restful {
    namespace Http {

        class HttpClient : public QObject {
            ///Q_OBJECT

        public:
            HttpClient(QObject *parent = 0) {
                m_manager = new QNetworkAccessManager();
            }

        public:
            HttpResponse* send(HttpRequest* request) {
                QNetworkRequest networkRequest(request->url());

                QMap<QString, QString> headers = request->headers();
                foreach (QString key, headers.keys()) {
                    networkRequest.setRawHeader(
                        key.toUtf8(),
                        QString(headers.value(key)).toUtf8()
                    );
                }

                QNetworkReply *networkReply;
                if (request->method() == "GET") {
                    networkReply = m_manager->get(networkRequest);
                } else if (request->method() == "POST") {
                    networkReply = m_manager->post(networkRequest, request->body()->toByteArray());
                } else if (request->method() == "PUT") {
                    networkReply = m_manager->put(networkRequest, request->body()->toByteArray());
                } else if (request->method() == "DELETE") {
                    networkReply = m_manager->deleteResource(networkRequest);
                } else {
                    throw "Unknown or unimplemented HTTP method '" + request->method() + "'";
                }

                QTimer timer;
                QEventLoop loop;

                connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
                connect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));

                timer.setSingleShot(true);
                timer.start(INT_MAX); // 120 seconds

                loop.exec();

                HttpResponse *response;
                if (timer.isActive()) {
                    timer.stop();
                    response = new HttpResponse(networkReply);
                } else {
                    networkReply->abort();
                    response = new HttpResponse(500);
                }

                disconnect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
                return response;
            }
            void sslCheck() {
                qDebug() << QSslSocket::supportsSsl() << ", QSslSocket =" << QSslSocket::sslLibraryBuildVersionString();
            }

        private:
            QNetworkAccessManager *m_manager;
        };

    };
};

#endif
