#include <qbsp/qbsp.hh>
#include <common/settings.hh>
#include <common/log.hh>

int main(int argc, const char **argv)
{
    logging::preinitialize();

    try
    {
        return qbsp_main(argc, argv);
    }
    catch (const settings::quit_after_help_exception &)
    {
        return 0;
    }
    catch (const std::exception &e)
    {
        exit_on_exception(e);
    }
}
