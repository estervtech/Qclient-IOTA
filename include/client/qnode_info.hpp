#pragma once

#include"client/qnode_response.hpp"
#include<QByteArray>
#include <QJsonValue>
#include <QJsonObject>
namespace qiota{


	class QCLIENT_EXPORT Node_info : public QObject
	{
		Q_OBJECT
		public:
			Node_info(Response*);
			QJsonObject amount_json(const quint64& amount_);

			quint64 network_id_;
			quint8  protocol_version,decimals;
			quint32 min_pow_score;
			QString bech32Hrp,unit,subunit;
			quint64 vByteFactorKey,vByteFactorData,vByteCost;
			bool isHealthy,pow_feature;
			void fill(QJsonValue data);
		signals:
			void finished(void);
		private:
			Response* response_;
			QByteArray network_name_;
	};


};
