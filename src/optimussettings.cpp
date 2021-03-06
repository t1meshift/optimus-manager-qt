/*
 *  Copyright © 2019 Hennadii Chernyshchyk <genaloner@gmail.com>
 *
 *  This file is part of Optimus Manager Qt.
 *
 *  Optimus Manager Qt is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a get of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "optimussettings.h"
#include "daemonclient.h"

#include <QFile>

const QMap<bool, QString> OptimusSettings::s_boolMap = {{false, QStringLiteral("no")},
                                                        {true, QStringLiteral("yes")}};
const QMap<OptimusSettings::SwitchingMethod, QString> OptimusSettings::s_switchingMethodMap = {{NoneMethod, QStringLiteral("none")},
                                                                                               {Nouveau, QStringLiteral("nouveau")},
                                                                                               {Bbswitch, QStringLiteral("bbswitch")},
                                                                                               {AcpiCall, QStringLiteral("acpi_call")}};
const QMap<OptimusSettings::PciReset, QString> OptimusSettings::s_pciResetMap = {{NoReset, s_boolMap[false]},
                                                                                 {FunctionLevelReset, QStringLiteral("function_level")},
                                                                                 {HotReset, QStringLiteral("hot_reset")}};
const QMap<OptimusSettings::Driver, QString> OptimusSettings::s_driverMap = {{Modesetting, QStringLiteral("modesetting")},
                                                                             {IntelDriver, QStringLiteral("intel")}};
const QMap<OptimusSettings::AccelMethod, QString> OptimusSettings::s_accelMethodMap = {{DefaultMethod, {}},
                                                                                       {SNA, QStringLiteral("sna")},
                                                                                       {XNA, QStringLiteral("xna")}};
const QMap<OptimusSettings::TearFree, QString> OptimusSettings::s_tearFreeMap = {{DefaultTearFree, {}},
                                                                                 {EnableTearFree, s_boolMap[true]},
                                                                                 {DisableTearFree, s_boolMap[false]}};
const QMap<OptimusSettings::NvidiaOption, QString> OptimusSettings::s_nvidiaOptionMap = {{Overclocking, QStringLiteral("overclocking")},
                                                                                         {TripleBuffer, QStringLiteral("triple_buffer")}};

OptimusSettings::OptimusSettings(QObject *parent)
    : QSettings(detectConfigPath().first, QSettings::IniFormat, parent)
{
}

OptimusSettings::OptimusSettings(const QString &filename, QObject *parent) :
    QSettings(filename, QSettings::IniFormat, parent)
{
}

OptimusSettings::SwitchingMethod OptimusSettings::switchingMethod() const
{
    const QString method = value("optimus/switching").toString();
    return s_switchingMethodMap.key(method, defaultSwitchingMethod());
}

void OptimusSettings::setSwitchingMethod(OptimusSettings::SwitchingMethod method)
{
    setValue("optimus/switching", s_switchingMethodMap[method]);
}

OptimusSettings::SwitchingMethod OptimusSettings::defaultSwitchingMethod()
{
    return NoneMethod;
}

bool OptimusSettings::isPciPowerControlEnabled() const
{
    const QString powerControlString = value("optimus/pci_power_control").toString();
    return s_boolMap.key(powerControlString, defaultPciPowerControlEnabled());
}

void OptimusSettings::setPciPowerControlEnabled(bool enable)
{
    setValue("optimus/pci_power_control", s_boolMap[enable]);
}

bool OptimusSettings::defaultPciPowerControlEnabled()
{
    return false;
}

bool OptimusSettings::isPciRemoveEnabled() const
{
    const QString pciRemoveString = value("optimus/pci_remove").toString();
    return s_boolMap.key(pciRemoveString, defaultPciRemoveEnabled());
}

void OptimusSettings::setPciRemoveEnabled(bool enable)
{
    setValue("optimus/pci_remove", s_boolMap[enable]);
}

bool OptimusSettings::defaultPciRemoveEnabled()
{
    return false;
}

OptimusSettings::PciReset OptimusSettings::pciReset() const
{
    const QString pciResetString = value("optimus/pci_reset").toString();
    return s_pciResetMap.key(pciResetString, defaultPciReset());
}

void OptimusSettings::setPciReset(PciReset type)
{
    setValue("optimus/pci_reset", s_pciResetMap[type]);
}

OptimusSettings::PciReset OptimusSettings::defaultPciReset()
{
    return NoReset;
}

bool OptimusSettings::isAutoLogoutEnabled() const
{
    const QString terminateSessionsString = value("optimus/auto_logout").toString();
    return s_boolMap.key(terminateSessionsString, defaultAutoLogoutEnabled());
}

void OptimusSettings::setAutoLogoutEnabled(bool enable)
{
    setValue("optimus/auto_logout", s_boolMap[enable]);
}

bool OptimusSettings::defaultAutoLogoutEnabled()
{
    return true;
}

OptimusSettings::Driver OptimusSettings::intelDriver() const
{
    const QString driverString = value("intel/driver").toString();
    return s_driverMap.key(driverString, defaultIntelDriver());
}

void OptimusSettings::setIntelDriver(OptimusSettings::Driver driver)
{
    setValue("intel/driver", s_driverMap[driver]);
}

OptimusSettings::Driver OptimusSettings::defaultIntelDriver()
{
    return Modesetting;
}

OptimusSettings::AccelMethod OptimusSettings::intelAccelMethod() const
{
    const QString accelMethodString = value("intel/accel").toString();
    return s_accelMethodMap.key(accelMethodString, defaultIntelAccelMethod());
}

void OptimusSettings::setIntelAccelMethod(OptimusSettings::AccelMethod method)
{
    setValue("intel/accel", s_accelMethodMap[method]);
}

OptimusSettings::AccelMethod OptimusSettings::defaultIntelAccelMethod()
{
    return DefaultMethod;
}

OptimusSettings::TearFree OptimusSettings::intelTearFree() const
{
    const QString tearFreeString = value("intel/tearfree").toString();
    return s_tearFreeMap.key(tearFreeString, defaultIntelTearFree());
}

void OptimusSettings::setIntelTearFree(OptimusSettings::TearFree tearFree)
{
    setValue("intel/tearfree", s_tearFreeMap[tearFree]);
}

OptimusSettings::TearFree OptimusSettings::defaultIntelTearFree()
{
    return DefaultTearFree;
}

OptimusSettings::DRI OptimusSettings::intelDri() const
{
    return value("intel/DRI", defaultIntelDri()).value<DRI>();
}

void OptimusSettings::setIntelDri(OptimusSettings::DRI dri)
{
    setValue("intel/DRI", dri);
}

OptimusSettings::DRI OptimusSettings::defaultIntelDri()
{
    return DRI3;
}

bool OptimusSettings::isIntelModesetEnabled() const
{
    const QString modesetString = value("intel/modeset").toString();
    return s_boolMap.key(modesetString, defaultIntelModesetEnabled());
}

void OptimusSettings::setIntelModesetEnabled(bool enabled)
{
    setValue("intel/modeset", s_boolMap[enabled]);
}

bool OptimusSettings::defaultIntelModesetEnabled()
{
    return true;
}

bool OptimusSettings::isNvidiaModesetEnabled() const
{
    const QString modesetString = value("nvidia/modeset").toString();
    return s_boolMap.key(modesetString, defaultNvidiaModesetEnabled());
}

void OptimusSettings::setNvidiaModesetEnabled(bool enabled)
{
    setValue("nvidia/modeset", s_boolMap[enabled]);
}

bool OptimusSettings::defaultNvidiaModesetEnabled()
{
    return true;
}

bool OptimusSettings::isNvidiaPatEnabled() const
{
    const QString patString = value("nvidia/PAT").toString();
    return s_boolMap.key(patString, defaultNvidiaPatEnabled());
}

void OptimusSettings::setNvidiaPatEnabled(bool enabled)
{
    setValue("nvidia/PAT", s_boolMap[enabled]);
}

bool OptimusSettings::defaultNvidiaPatEnabled()
{
    return true;
}

int OptimusSettings::nvidiaDpi() const
{
    return value("nvidia/DPI", defaultNvidiaDpi()).toInt();
}

void OptimusSettings::setNvidiaDpi(int dpi)
{
    if (dpi == 0)
        setValue("nvidia/DPI", {});
    else
        setValue("nvidia/DPI", dpi);
}

int OptimusSettings::defaultNvidiaDpi()
{
    return 96;
}

bool OptimusSettings::isNvidiaIgnoreAbi() const
{
    const QString ignoreAbiString = value("nvidia/ignore_abi").toString();
    return s_boolMap.key(ignoreAbiString, defaultNvidiaIgnoreAbi());
}

void OptimusSettings::setNvidiaIgnoreAbi(bool ignore)
{
    setValue("nvidia/ignore_abi", s_boolMap[ignore]);
}

bool OptimusSettings::defaultNvidiaIgnoreAbi()
{
    return false;
}

OptimusSettings::NvidiaOptions OptimusSettings::nvidiaOptions() const
{
    const QStringList optionStrings = value("nvidia/options", nvidiaOptionsToStrings(defaultNvidiaOptions())).toStringList();
    return stringToNvidiaOptions(optionStrings);
}

void OptimusSettings::setNvidiaOptions(NvidiaOptions options)
{
    setValue("nvidia/options", nvidiaOptionsToStrings(options));
}

OptimusSettings::NvidiaOptions OptimusSettings::defaultNvidiaOptions()
{
    return Overclocking;
}

QString OptimusSettings::permanentConfigPath()
{
    return QStringLiteral("/etc/optimus-manager/optimus-manager.conf");
}

QPair<QString, OptimusSettings::ConfigType> OptimusSettings::detectConfigPath()
{
    QFile tempPath("/var/lib/optimus-manager/temp_conf_path");
    if (tempPath.open(QIODevice::ReadOnly))
        return {tempPath.readAll(), Temporary};

    return {permanentConfigPath(), Permanent};
}

OptimusSettings::ConfigType OptimusSettings::defaultConfigType()
{
    return Permanent;
}

QStringList OptimusSettings::nvidiaOptionsToStrings(NvidiaOptions options)
{
    QStringList optionStrings;
    for (auto it = s_nvidiaOptionMap.cbegin(); it != s_nvidiaOptionMap.cend(); ++it) {
        if (options.testFlag(it.key()))
            optionStrings.append(it.value());
    }

    return optionStrings;
}

OptimusSettings::NvidiaOptions OptimusSettings::stringToNvidiaOptions(const QStringList &optionStrings)
{
    NvidiaOptions options;
    for (auto it = s_nvidiaOptionMap.cbegin(); it != s_nvidiaOptionMap.cend(); ++it) {
        if (optionStrings.contains(it.value()))
            options |= it.key();
    }

    return options;
}
