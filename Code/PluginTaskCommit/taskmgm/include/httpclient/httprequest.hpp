#ifndef _HTTP_REQUEST_HPP
#define _HTTP_REQUEST_HPP

#include <QObject>
#include <QUrl>
#include <QMap>

#include "httpbody.hpp"

namespace Restful {
    namespace Http {

        class HttpRequest : public QObject {
            ///Q_OBJECT

        public:
            HttpRequest(QObject *parent = 0) {
                m_body = new HttpBody();
            }

        public:
            void setHeaders(QMap<QString, QString> headers) {
                m_headers = headers;
            }
            QMap<QString, QString> headers() {
                return m_headers;
            }

            void setMethod(QString method) {
                m_method = method;
            }
            QString method() {
                return m_method;
            }

            void setUrl(QUrl url) {
                m_url = url;
            }
            QUrl url() {
                return m_url;
            }

            void setBody(HttpBody* body) {
                m_body = body;
            }
            HttpBody* body() {
                return m_body;
            }

        private:
            QMap<QString, QString> m_headers;
            QString m_method;
            QUrl m_url;
            HttpBody* m_body;
        };

    };
};

#endif
