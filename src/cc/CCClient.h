/* XMRigCC
 * Copyright 2017-     BenDr0id    <ben@graef.in>
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CC_CLIENT_H__
#define __CC_CLIENT_H__

#ifndef XMRIG_NO_CC

#include <uv.h>
#include <chrono>
#include <ctime>
#include <3rdparty/cpp-httplib/httplib.h>
#include "ClientStatus.h"
#include "version.h"

#ifdef TYPE_AMD_GPU
#include "amd/GpuContext.h"
#include "core/Controller.h"
#else
#include "Options.h"
#endif

namespace xmrig {
    class NetworkState;
}

class Hashrate;

class CCClient
{
public:
#ifdef TYPE_AMD_GPU
    CCClient(xmrig::Config* m_config, uv_async_t* async);
    static void updateGpuInfo(const std::vector<GpuContext *> gpuContexts);
#else
    CCClient(Options* config, uv_async_t* async);
#endif

    ~CCClient();

    static void updateHashrate(const Hashrate *hashrate);
    static void updateNetworkState(const xmrig::NetworkState &results);

private:

    void publishClientStatusReport();
    void updateConfig();
    void publishConfig();
    void refreshUptime();
    void refreshLog();

    std::shared_ptr<httplib::Response> performRequest(const std::string& requestUrl,
                                                             const std::string& requestBuffer,
                                                             const std::string& operation);
    static void onThreadStarted(void *handle);
    static void onReport(uv_timer_t *handle);

#ifdef TYPE_AMD_GPU
    const xmrig::Config* m_config;
#else
    const Options* m_config;
#endif

    static CCClient* m_self;
    static uv_mutex_t m_mutex;

    ClientStatus m_clientStatus;

    std::string m_authorization;

    std::chrono::time_point<std::chrono::system_clock> m_startTime;

    uv_async_t* m_async;
    uv_timer_t m_timer;
    uv_loop_t m_client_loop;
    uv_thread_t m_thread;
};

#endif
#endif /* __CC_CLIENT_H__ */
