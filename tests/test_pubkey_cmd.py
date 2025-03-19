import pytest

from application_client.boilerplate_command_sender import BoilerplateCommandSender, Errors
from application_client.boilerplate_response_unpacker import unpack_get_public_key_response
from ragger.bip import calculate_public_key_and_chaincode, CurveChoice
from ragger.error import ExceptionRAPDU
from ragger.navigator import NavInsID, NavIns


# In this test we check that the GET_PUBLIC_KEY works in non-confirmation mode
def test_get_public_key_no_confirm(backend):
    path = "m/44'/1'/0'/0/0"
    client = BoilerplateCommandSender(backend)
    response = client.get_public_key(path=path).data
    _, public_key = unpack_get_public_key_response(response)

    assert public_key == b'X\xbc\xec\xf9qm\x06N?E\x8a\x847\x9co"\x8fl\x8e\xce\xccle}U\x1aH\xcb?\x9dB'


# In this test we check that the GET_PUBLIC_KEY works in confirmation mode
def test_get_public_key_confirm_accepted(backend, scenario_navigator):

    client = BoilerplateCommandSender(backend)
    path = "m/44'/1'/0'/0/0"
    with client.get_public_key_with_confirmation(path=path):
        scenario_navigator.address_review_approve()

    response = client.get_async_response().data
    _, public_key = unpack_get_public_key_response(response)

    assert public_key == b'X\xbc\xec\xf9qm\x06N?E\x8a\x847\x9co"\x8fl\x8e\xce\xccle}U\x1aH\xcb?\x9dB'


# In this test we check that the GET_PUBLIC_KEY in confirmation mode replies an error if the user refuses
def test_get_public_key_confirm_refused(backend, scenario_navigator):

    client = BoilerplateCommandSender(backend)
    path = "m/44'/1'/0'/0/0"

    with pytest.raises(ExceptionRAPDU) as e:
        with client.get_public_key_with_confirmation(path=path):
            scenario_navigator.address_review_reject()

    # Assert that we have received a refusal
    assert e.value.status == Errors.SW_DENY
    assert len(e.value.data) == 0
