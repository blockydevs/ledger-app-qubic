import pytest

from application_client.boilerplate_transaction_qubic import TransactionQubic
from application_client.boilerplate_command_sender import BoilerplateCommandSender, Errors
from application_client.boilerplate_response_unpacker import unpack_get_public_key_response, unpack_sign_tx_response
from ragger.error import ExceptionRAPDU
from ragger.navigator import NavInsID, NavIns

from utils import check_signature_validity

def test_sign_tx_expert_mode(firmware, backend, scenario_navigator, navigator):

    # given

    if firmware.name == 'flex':
        instructions = [
            NavInsID.USE_CASE_HOME_SETTINGS,
            NavIns(NavInsID.TOUCH, (200, 113)),
            NavIns(NavInsID.TOUCH, (200, 300)),
            NavIns(NavInsID.TOUCH, (200, 400))
        ]
    elif firmware.name == 'stax':
        instructions = [
            NavInsID.USE_CASE_HOME_SETTINGS,
            NavIns(NavInsID.TOUCH, (200, 113)),
            NavIns(NavInsID.TOUCH, (200, 261)),
            NavIns(NavInsID.TOUCH, (200, 400))
        ]
    else:
        instructions = [
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK
        ]

    navigator.navigate(instructions,
                       screen_change_before_first_instruction=False)

    client = BoilerplateCommandSender(backend)
    path: str = "m/44'/1'/0'/0/0"

    rapdu = client.get_public_key(path=path)
    _, public_key, _, _ = unpack_get_public_key_response(rapdu.data)

    string_private_key = (b'\x04K\xfe\xc5||\xfbQR\xd6HD\x9e\x90\xf0\xfco\x8b;\xcf\x9bu\xc6\xef\xb1\xea\x81'
                          b'\xd9\x03a\x02\x97o\x8f\xbf\xcfT\x9c?0\x03\nk\xa3:\xbd\x1b6\
                          xeeC:Bg\xf7B\x14\xf9\x84m\xa6A\xf8\x03g\xba')

    example_transaction = TransactionQubic(
        source_public_key=string_private_key,
        destination_public_key=string_private_key,
        amount=123,
        tick=12345,
        input_type=1,
        input_size=256
    )
    transaction_qubic = example_transaction.serialize()

    # when

    with client.sign_tx(path=path, transaction=transaction_qubic):
        scenario_navigator.review_approve()

    # then

    response = client.get_async_response().data
    _, der_sig, _ = unpack_sign_tx_response(response)
    assert check_signature_validity(public_key, der_sig, transaction_qubic)

def test_sign_tx_short_tx(backend, scenario_navigator):

    # given

    client = BoilerplateCommandSender(backend)
    path: str = "m/44'/1'/0'/0/0"

    rapdu = client.get_public_key(path=path)
    _, public_key, _, _ = unpack_get_public_key_response(rapdu.data)

    string_private_key = (b'\x04K\xfe\xc5||\xfbQR\xd6HD\x9e\x90\xf0\xfco\x8b;\xcf\x9bu\xc6\xef\xb1\xea\x81'
                          b'\xd9\x03a\x02\x97o\x8f\xbf\xcfT\x9c?0\x03\nk\xa3:\xbd\x1b6\
                          xeeC:Bg\xf7B\x14\xf9\x84m\xa6A\xf8\x03g\xba')

    example_transaction = TransactionQubic(
        source_public_key=string_private_key,
        destination_public_key=string_private_key,
        amount=123,
        tick=12345,
        input_type=1,
        input_size=256
    )
    transaction_qubic = example_transaction.serialize()

    # when

    with client.sign_tx(path=path, transaction=transaction_qubic):
        scenario_navigator.review_approve()

    # then

    response = client.get_async_response().data
    _, der_sig, _ = unpack_sign_tx_response(response)
    assert check_signature_validity(public_key, der_sig, transaction_qubic)

def test_sign_tx_short_tx_blind_sign(firmware, navigator, backend, scenario_navigator, test_name, default_screenshot_path):

    # given

    if firmware.name == 'flex':
        instructions = [
            NavInsID.USE_CASE_HOME_SETTINGS,
            NavIns(NavInsID.TOUCH, (200, 113)),
            NavIns(NavInsID.TOUCH, (200, 300)),
            NavIns(NavInsID.TOUCH, (200, 400))
        ]
    elif firmware.name == 'stax':
        instructions = [
            NavInsID.USE_CASE_HOME_SETTINGS,
            NavIns(NavInsID.TOUCH, (200, 113)),
            NavIns(NavInsID.TOUCH, (200, 261)),
            NavIns(NavInsID.TOUCH, (200, 400))
        ]
    else:
        instructions = [
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK
        ]

    navigator.navigate(instructions,
                       screen_change_before_first_instruction=False)

    client = BoilerplateCommandSender(backend)
    path: str = "m/44'/1'/0'/0/0"

    rapdu = client.get_public_key(path=path)
    _, public_key, _, _ = unpack_get_public_key_response(rapdu.data)

    transaction = b'\x80\x03}q\x00X\x03\x00\x00\x00fooq\x01X\x03\x00\x00\x00barq\x02s.'

    # when

    if firmware.is_nano:
        with client.sign_tx(path=path, transaction=transaction):
            navigator.navigate_and_compare(default_screenshot_path,
                                            test_name+"/part1",
                                            [NavInsID.RIGHT_CLICK],
                                            screen_change_after_last_instruction=False)
            scenario_navigator.review_approve()

    else:
        with client.sign_tx(path=path, transaction=transaction):
            navigator.navigate_and_compare(default_screenshot_path,
                                           test_name+"/part1",
                                           [NavInsID.USE_CASE_CHOICE_REJECT],
                                           screen_change_after_last_instruction=False)
            scenario_navigator.review_approve()

    # then

    response = client.get_async_response().data
    _, der_sig, _ = unpack_sign_tx_response(response)
    assert check_signature_validity(public_key, der_sig, transaction)

def test_sign_tx_long_tx(backend, scenario_navigator):

    # given

    client = BoilerplateCommandSender(backend)
    path: str = "m/44'/1'/0'/0/0"

    rapdu = client.get_public_key(path=path)
    _, public_key, _, _ = unpack_get_public_key_response(rapdu.data)

    string_private_key = (b'\x04K\xfe\xc5||\xfbQR\xd6HD\x9e\x90\xf0\xfco\x8b;\xcf\x9bu\xc6\xef\xb1\xea\x81'
                          b'\xd9\x03a\x02\x97o\x8f\xbf\xcfT\x9c?0\x03\nk\xa3:\xbd\x1b6\
                          xeeC:Bg\xf7B\x14\xf9\x84m\xa6A\xf8\x03g\xba')

    example_transaction = TransactionQubic(
        source_public_key=string_private_key,
        destination_public_key=string_private_key,
        amount=123,
        tick=12345,
        input_type=1,
        input_size=256
    )
    transaction_qubic = example_transaction.serialize()

    # when

    with client.sign_tx(path=path, transaction=transaction_qubic):
        scenario_navigator.review_approve()

    # then

    response = client.get_async_response().data
    _, der_sig, _ = unpack_sign_tx_response(response)
    assert check_signature_validity(public_key, der_sig, transaction_qubic)

def test_sign_tx_refused(backend, scenario_navigator):

    # given

    client = BoilerplateCommandSender(backend)
    path: str = "m/44'/1'/0'/0/0"

    rapdu = client.get_public_key(path=path)
    _, pub_key, _, _ = unpack_get_public_key_response(rapdu.data)

    string_private_key = (b'\x04K\xfe\xc5||\xfbQR\xd6HD\x9e\x90\xf0\xfco\x8b;\xcf\x9bu\xc6\xef\xb1\xea\x81'
                          b'\xd9\x03a\x02\x97o\x8f\xbf\xcfT\x9c?0\x03\nk\xa3:\xbd\x1b6\
                          xeeC:Bg\xf7B\x14\xf9\x84m\xa6A\xf8\x03g\xba')

    example_transaction = TransactionQubic(
        source_public_key=string_private_key,
        destination_public_key=string_private_key,
        amount=123,
        tick=12345,
        input_type=1,
        input_size=256
    )
    transaction_qubic = example_transaction.serialize()

    # when

    with pytest.raises(ExceptionRAPDU) as e:
        with client.sign_tx(path=path, transaction=transaction_qubic):
            scenario_navigator.review_reject()

    # then

    assert e.value.status == Errors.SW_DENY
    assert len(e.value.data) == 0
