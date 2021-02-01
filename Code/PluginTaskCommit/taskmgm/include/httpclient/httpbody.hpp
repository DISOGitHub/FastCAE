#ifndef _HTTP_BODY_HPP
#define _HTTP_BODY_HPP

#include <QObject>

namespace Restful {
    namespace Http {

        class HttpBody : public QObject {
            ///Q_OBJECT

        public:
            HttpBody(QObject *parent = 0) {
                m_body = "";
            }
            HttpBody(QString body, QObject *parent = 0) {
                m_body = body.toLocal8Bit();
            }
            HttpBody(QByteArray body, QObject *parent = 0) {
                m_body = body;
            }

        public:
            QString toString() {
                return m_body;
            }
            QByteArray toByteArray() {
                return m_body;
            }
            int length() {
                return m_body.length();
            }

        public:
            bool isEmpty() {
                return m_body.isEmpty();
            }

        private:
            QByteArray m_body;
        };

    };
};

#endif
