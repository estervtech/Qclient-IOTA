#include"client/qnode_info.hpp"
#include<QDataStream>
#include<QJsonArray>
#include<QDebug>
#include <QCryptographicHash>
namespace qiota{


Node_info::Node_info(Response* resp):response_(resp),pow_feature(false),isHealthy(false)
{
    QObject::connect(resp, &Response::returned,this, &Node_info::fill);
}
void Node_info::fill(QJsonValue data)
{
    network_name_=QByteArray(data["protocol"].toObject()["networkName"].toString().toLatin1());
    QByteArray networkId_hash=QCryptographicHash::hash(network_name_,QCryptographicHash::Blake2b_256);
    networkId_hash.truncate(8);
    auto buffer=QDataStream(&networkId_hash,QIODevice::ReadOnly);
    buffer.setByteOrder(QDataStream::LittleEndian);
    buffer>>network_id_;

    protocol_version=(data["protocol"].toObject())["version"].toInt();
    min_pow_score=(data["protocol"].toObject())["minPowScore"].toInteger();
    bech32Hrp=(data["protocol"].toObject())["bech32Hrp"].toString();

    const auto rentStructure=(data["protocol"].toObject())["rentStructure"].toObject();
    vByteFactorKey=rentStructure["vByteFactorKey"].toInteger();
    vByteFactorData=rentStructure["vByteFactorData"].toInteger();
    vByteCost=rentStructure["vByteCost"].toInteger();

    const auto baseToken=(data["baseToken"].toObject());
    unit=baseToken["unit"].toString();
    subunit=baseToken["subunit"].toString();
    decimals=baseToken["decimals"].toInt();

    const auto features=data["features"].toArray();
    for(const auto v:features){if(v=="pow"){pow_feature=true;}}


    isHealthy=data["status"].toObject()["isHealthy"].toBool();

    emit finished();
    response_->deleteLater();
}

QJsonObject Node_info::amount_json(const quint64& amount_)
{
    QJsonObject shortValue;
    shortValue.insert("unit",unit);
    shortValue.insert("value",QString::number(amount_*1.0/std::pow(10,decimals),'g', 5));

    QJsonObject largeValue;
    largeValue.insert("unit",subunit);
    largeValue.insert("value",QString::number(amount_));
    QJsonObject var;
    var.insert("shortValue",shortValue);
    var.insert("largeValue",largeValue);

    if(amount_>std::pow(10,decimals*0.8))
    {
        var.insert("default",0);
    }
    else
    {
        var.insert("default",1);
    }
    return var;
}

}
