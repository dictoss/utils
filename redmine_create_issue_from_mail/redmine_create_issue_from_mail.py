#!/usr/bin/python3
#
# create issue for redmine script.
#
#  for debian:
#   # apt-get installl python3-pip
#   # pip3 install python-redmine==2.2.1

import sys
from redminelib import Redmine


class MyRedmineApi(object):
    """
    My Redmine API client class
    """
    def __init__(self, url, token, project_name=''):
        self.url = url
        self.token = token
        self._project_name = project_name

        self._rdm = Redmine(url, key=token)
        self._rdm_project = None

    def get_project(self, name=''):
        """
        get project id from project name by call redmine api.
        """
        target_name = ''

        if name:
            target_name = name
        else:
            target_name = self._project_name

        if not target_name:
            return None
        
        try:
            self._rdm_project = self._rdm.project.get(target_name)
        except Exception as e:
            print('EXCEPT: {}'.format(e))
            return None

        return self._rdm_project

    def create_issue(self, issue_data, rdm_project):
        return True

    def extract_issue_data_from_mail(self, mailfile):
        _issue_data = {'id': 0}

        return _issue_data



def main(redmine_url, redmine_token, redmine_project_name, mailfilelist):

    _redmine = MyRedmineApi(redmine_url, redmine_token, redmine_project_name)

    # convert project id from name
    try:
        _proj = _redmine.get_project()
    except Exception as e:
        print('EXCEPT: {}'.format(e))
        return 1

    # create issues from mailfile.
    for m in mailfilelist:
        _issue_data = _redmine.extract_issue_data_from_mail(m)
        if _issue_data is not None:
            _result = _redmine.create_issue(_issue_data, _proj)
            if _result is True:
                print('SUCCESS create issue (project={}, id={})'.format(
                    _proj.name, _issue_data['id']))

    return 0


def usage():
    s = """This script create issue for redmine from mail.

usage: {} [redmine_url] [redmine_token] [mailfile...] 
""".format(sys.argv[0])
    print(s, file=sys.stderr)


if __name__ == '__main__':
   ret = 0

   if len(sys.argv) < 5:
       usage()
       sys.exit(4)
   else:
       _redmine_url = sys.argv[1]
       _redmine_token = sys.argv[2]
       _redmine_project_name = sys.argv[3]
       _mailfilelist = sys.argv[4:]

       ret = main(_redmine_url, _redmine_token,
           _redmine_project_name,_mailfilelist)

       sys.exit(ret)

