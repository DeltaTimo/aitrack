#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !_DEBUG


#include <QtWidgets/QApplication>
#include "view/WindowMain.h"
#include "presenter/presenter.h"
#include "model/Config.h"
#include <omp.h>
#include "tracker/TrackerFactory.h"

#include "model/UpdateChecker.h"



int main(int argc, char *argv[])
{

#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    auto conf_mgr = std::make_unique<ConfigMgr>("./prefs.ini");

    auto state = conf_mgr->getConfig();
    if (state.onnx_set_env_threads) {
        std::wstring ws = std::to_wstring(state.onnx_env_threads);
        SetEnvironmentVariable(LPWSTR(L"OMP_NUM_THREADS"), ws.c_str());
    }
    if (state.onnx_set_num_threads) {
        omp_set_num_threads(state.onnx_num_threads);
    }
    if (state.onnx_set_dynamic) {
        omp_set_dynamic(state.onnx_dynamic);
    }

    QApplication app(argc, argv);

    WindowMain w;
    w.show();

    auto t_factory = std::make_unique<TrackerFactory>("./models/");

    Presenter p((IView&)w, std::move(t_factory), std::move(conf_mgr));

    return app.exec();
}
