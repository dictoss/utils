#!/usr/bin/python3
#
# create issue for redmine script.
#
#  for debian:
#   # apt-get installl python3-pip
#   # pip3 install python-redmine==2.2.1

import sys
import datetime
from email.message import EmailMessage
from email.parser import BytesParser
from email.header import decode_header
from email.utils import parsedate_tz

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
        _result = issue_data.save()
        return _result

    def extract_issue_data_from_mail(self, mailfile):
        _issue = self._rdm.issue.new()
        _maildata = None

        # parse email
        try:
             _parser = BytesParser()
             with open(mailfile, mode='br') as f:
                _maildata = _parser.parse(f)
        except Exception as e:
            print('EXCEPT 1: {}'.format(e))
            return None

        #
        # set issue
        #
        _issue.project_id = self._rdm_project.identifier

        _issue.subject = self._get_mail_headler('Subject', _maildata)
        _issue.description = self._get_mail_body(_maildata)

        _issue.tracker_id = self._get_tracker_id(_maildata)
        _issue.status_id = self._get_status_id(_maildata)
        _issue.priority_id = self._get_priority_id(_maildata)

        # no set
        _issue.start_date = None
        _issue.due_date = None
        _issue.assigned_to_id = None
        _issue.parent_issue_id = None
        _issue.watcher_user_ids = []
        _issue.estimated_hours = None
        _issue.done_ratio = None
        _issue.uploads = []

        # at your own set.
        _issue.custom_fields = []

        return _issue

    def _get_tracker_id(self, maildata):
        return 1

    def _get_status_id(self, maildata):
        return 1

    def _get_priority_id(self, maildata):
        return 1

    def _get_mail_headler(self, key, maildata):
        _s = ''
        _h = maildata.get(key)
        _t = decode_header(_h)

        for t in _t:
            _charset = t[1]

            if type(t[0]) is bytes:
                if _charset:
                    _s += t[0].decode(_charset)
                else:
                    _s += t[0].decode()
            elif type(tup[0]) is str:
                _s += t[0]
            else:
                pass

        return _s

    def _get_mail_body(self, maildata):
        """
        FIXME: support multipart
        """
        _charset = maildata.get_content_charset()
        _payload = maildata.get_payload(decode=True)

        try:
            if _payload:
                if _charset:
                   _s = _payload.decode(_charset)
                else:
                   _s = _payload.decode()
            else:
                _s = ""
        except:
           return _payload

        return _s


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
            if _result:
                print('SUCCESS create issue. (project={}, id={})'.format(
                    _proj.identifier, _issue_data['id']))
            else:
                print('FAIL create issue ! (project={}, mailfile={})'.format(
                    _proj.identifier, m))


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

