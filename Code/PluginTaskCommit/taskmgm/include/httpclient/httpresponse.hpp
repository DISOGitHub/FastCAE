#ifndef _HTTP_RESPONSE_HPP
#define _HTTP_RESPONSE_HPP

#include <QObject>
#include <QNetworkReply>

#include "httpbody.hpp"

namespace Restful {
    namespace Http {

        class HttpResponse : public QObject {
            ///Q_OBJECT

        public:
            HttpResponse(int status, QObject *parent = 0) {
                m_status = status;
                m_body = new HttpBody(QString("request time out"));

            }
            HttpResponse(int status, QMap<QString, QString> headers, HttpBody *body, QObject *parent = 0) {
                m_status = status;
                m_headers = headers;
                m_body = body;
            }
            HttpResponse(QNetworkReply *reply, QObject *parent = 0) {
                m_status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                m_data = reply->readAll();
                m_body = new HttpBody(m_data);

                QList<QByteArray> headers = reply->rawHeaderList();
                foreach (QByteArray key, headers) {
                    m_headers.insert(QString(key), reply->rawHeader(key));
                }
            }

        public:
            int status() {
                return m_status;
            }
            QMap<QString, QString> headers() {
                return m_headers;
            }
            HttpBody* body() {
                return m_body;
            }
            QByteArray data(){
                return m_data;
            }

        public:
            bool isValid() {
                return status() >= 100 && status() < 600;
            }
            bool isInformational() {
                return status() >= 100 && status() < 200;
            }
            bool isSuccessful() {
                return status() >= 200 && status() < 300;
            }
            bool isRedirection() {
                return status() >= 300 && status() < 400;
            }
            bool isClientError() {
                return status() >= 400 && status() < 500;
            }
            bool isServerError() {
                return status() >= 500 && status() < 600;
            }
            bool isOk() {
                return status() == 200;
            }
            bool isForbidden() {
                return status() == 403;
            }
            bool isNotFound() {
                return status() == 404;
            }
            bool isEmpty() {
                return status() == 204 || status() == 304;
            }

        private:
            int m_status;
            QMap<QString, QString> m_headers;
            HttpBody* m_body;
            QByteArray m_data;
        };

    };
};

#endif
